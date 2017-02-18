<?hh // strict
namespace Decouple\HTTP\Router;
use Decouple\HTTP\Router\Route\RouteInterface;
use Decouple\Decoupler\Decoupler;
use Decouple\HTTP\Request\Request;
class Router {
  public Vector<RouteInterface> $routes;
  public function __construct(private Decoupler $decoupler) {
    $this->routes = Vector {};
  }
  public function serve(string $type, string $pattern, mixed $function=null) : int {
    $route = null;
    if(is_callable($function)) {
      if(is_array($function)) {
        $route = new \Decouple\HTTP\Router\Route\MethodRoute($type, $pattern, $function);
      } else {
        $route = new \Decouple\HTTP\Router\Route\FunctionRoute($type, $pattern, $function);
      }
      return $this->add($route);
    } else if(is_string($function)) {
      if(class_exists($function, true)) {
        $route = new \Decouple\HTTP\Router\Route\RestfulRoute($pattern, $function);
        return $this->add($route);
      } else if(stristr($function, '@')) {
        /** Try to automagically load Class@method syntax **/
        list($class, $method) = explode('@', $function);
        if(!class_exists($class) && method_exists($class, $method)) {
          throw new \Exception(sprintf("Service is not a valid class: %s", $class));
        }
        $route = new \Decouple\HTTP\Router\Route\MethodRoute($type, $pattern, [$class,$method]);
        return $this->add($route);
      } else {
        throw new \Exception(sprintf("Non-existant class: %s", $function));
      }
    } else if(is_array($function)) {
      $first = array_shift($function);
      $method = array_shift($function);
      if(class_exists($first)) {
        throw new \Exception(sprintf("Method %s does not exist in class [%s]", $method, $first));
      } else if(is_object($first)) {
        throw new \Exception(sprintf("Method %s does not exist in object [%s]", $method, (string)get_class($first)));
      } else {
        throw new \Exception(sprintf("Invalid class/method provided for route %s [%s]", $pattern, $first));
      }
    }
    throw new \Exception(sprintf("Invalid route function provided for route %s [%s]", $pattern, $function));
  }
  public function add(RouteInterface $route) : void {
    $this->routes->add($route);
    return $this->routes->count();
  }
  public function fetch(int $id) : RouteInterface {
    return $this->routes->at($id);
  }
  public function get(string $pattern, mixed $function=null) : int {
    return $this->serve('GET', $pattern, $function);
  }
  public function post(string $pattern, mixed $function=null) : int {
    return $this->serve('POST', $pattern, $function);
  }
  public function delete(string $pattern, mixed $function=null) : int {
    return $this->serve('DELETE', $pattern, $function);
  }
  public function options(string $pattern, mixed $function=null) : int {
    return $this->serve('OPTIONS', $pattern, $function);
  }
  public function put(string $pattern, mixed $function=null) : int {
    return $this->serve('PUT', $pattern, $function);
  }
  public function head(string $pattern, mixed $function=null) : int {
    return $this->serve('HEAD', $pattern, $function);
  }
  public function restful(string $pattern, mixed $function=null) : int {
    return $this->serve('ANY', $pattern, $function);
  }
  public function group(Map<string,mixed> $options, Vector <Pair <string, Pair <string,mixed>>> $routes) : void {
    $prefix = '';
    $passing = true;
    if($options->contains('middleware')) {
      $passing = $this->decoupler->inject($passing);
    }
    if($passing) {
      if($options->contains('prefix')) {
        $prefix = (string)$options->get('prefix');
      }
      foreach($routes as $route) {
        $this->serve($route[0], $route[1][0], $route[1][1]);
      }
    }
  }
  public function route(Request $request) : mixed {
    foreach($this->routes as $route) {
      if($route->isValid($request->uri)) {
        $request->routeParams = $route->getParams($request->uri);
        return $route->execute($request);
      }
    }
    throw new \Exception(sprintf("Unable to find a route for url [%s]", $request->uri));
  }
}
