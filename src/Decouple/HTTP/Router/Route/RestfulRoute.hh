<?hh // strict
namespace Decouple\HTTP\Router\Route;
class RestfulRoute extends AbstractRoute implements RouteInterface {
  public function from(mixed $function) : RestfulRoute {
    $this->callback = $function;
    return $this;
  }
  public function execute(\Decouple\HTTP\Request\Request $request) : mixed {
    return [$this->callback,$request->routeParams[1]];
  }
}
