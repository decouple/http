<?hh // strict
namespace Decouple\HTTP\Router\Route;
use Decouple\HTTP\Request\Request;
use Decouple\HTTP\Request\Uri;
class RestfulRoute extends AbstractRoute implements RouteInterface {
  public string $method;
  public function __construct(public string $pattern, public mixed $callback=null) {
    $this->method = 'ANY';
    // Do nothing
  }

  public function isValid(Uri $uri) : bool {
    if(strrpos($uri, $this->pattern) === 0) {
      return true;
    } else if(parent::isValid($uri)) {
      return true;
    }
    return false;
  }

  public function getParams(Uri $uri) : Vector<string> {
    $matches = [];
    $result = Vector {};
    if($this->isValid($uri)) {
      if(empty($matches)) {
        $parts = explode('/', str_replace($this->pattern, '', $uri));
        $result->addAll($this->reduce($parts));
      } else {
        preg_match_all('|' . $this->pattern . '|', $uri, $matches);
        if(is_array($matches)) {
          $result->addAll($this->reduce($matches));
        }
      }
    }
    return $result;
  }

  public function from(mixed $function) : RestfulRoute {
    $this->callback = $function;
    return $this;
  }

  public function execute(\Decouple\HTTP\Request\Request $request) : mixed {
    if($request->routeParams->count()) {
      $route = $request->routeParams->at(0);
      $request->routeParams->splice(0, 1);
    } else {
      $route = 'index';
    }
    if(stristr($route, '/')) {
      $params = explode('/', $route);
      $route = array_shift($params);
      foreach($params as $param) {
        $request->routeParams->add($param);
      }
    } else {
      $params = [];
    }

    if(method_exists($this->callback, 'any' . ucfirst($route))) {
      $route = 'any' . ucfirst($route);
    } else {
      $route = strtolower($request->type) . ucfirst($route);
    }

    return [$this->callback,$route];
  }
}
