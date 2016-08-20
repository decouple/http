<?hh // strict
namespace Decouple\HTTP\Router\Route;
class RestfulRoute extends AbstractRoute implements RouteInterface {
  public string $method;
  public function __construct(public string $pattern, public mixed $callback=null) {
    $this->method = 'ANY';
    // Do nothing
  }

  public function from(mixed $function) : RestfulRoute {
    $this->callback = $function;
    return $this;
  }

  public function execute(\Decouple\HTTP\Request\Request $request) : mixed {
    $route = !($request->routeParams->at(1))?'index':$request->routeParams->at(1);
    if(stristr($route, '/')) {
      $params = explode('/', $route);
      $route = array_shift($params);
      foreach($params as $param) {
        $request->routeParams->add($param);
      }
    } else {
      $params = [];
    }
    return [$this->callback,$route];
  }
}
