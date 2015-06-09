<?hh // strict
namespace Decouple\HTTP\Router\Route;
class MethodRoute extends AbstractRoute implements RouteInterface {
  public function from(mixed $method) : MethodRoute {
    $this->callback = $method;
    return $this;
  }
  public function execute(\Decouple\HTTP\Request\Request $request) : mixed {
    return $this->callback;
  }
}
