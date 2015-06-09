<?hh // strict
namespace Decouple\HTTP;
use Decouple\HTTP\Request\Request;
use Decouple\HTTP\Router\Router;
use Decouple\Decoupler\Decoupler;
use Decouple\HTTP\Router\Route\AbstractRoute;
class App {
  public function __construct(public Request $request, protected Router $router, protected Decoupler $decoupler) {
  }
  public function execute() : mixed {
    $result = $this->router->route($this->request);
    if(is_array($result)) {
      $object = $this->decoupler->injectInstance($result[0]);
      return $this->decoupler->injectMethod($object, $result[1]);
    } else if(is_string($result) && stristr($result, '::')) {
      list($class,$method) = explode('::',$result);
      $object = $this->decoupler->injectInstance($class);
      return $this->decoupler->injectMethod($object, $method);
    } else if(is_callable($result)) {
      return $this->decoupler->injectFunction($result);
    } else {
      return $result;
    }
  }
  public function addServices(KeyedTraversable<string,mixed> $services) : void {
    foreach($services as $name => $service) {
      $this->addService($name, $service);
    }
  }
  public function addService(string $name, mixed $service=true) : void {
    if($service === true) {
      if(class_exists($name)) {
        $service = $this->decoupler->injectInstance($name);
      } else {
        throw new \Exception(sprintf("Unable to automagically create service.\n Class %s is undefined.", $name));
      }
    }
    if(!is_object($service)) {
      throw new \Exception(sprintf("Services registered with HTTP must be objects! [%s is set to %s]", $name, (string)$service));
    }
    $this->decoupler->set($name, $service);
  }
  public function addRoutes(KeyedTraversable<string,mixed> $routes) : void {
    foreach($routes as $pattern => $route) {
      $this->addRoute($pattern, $route);
    }
  }
  public function addRoute(string $pattern, mixed $route) : void {
    $this->router->serve($pattern, $route);
  }
}
