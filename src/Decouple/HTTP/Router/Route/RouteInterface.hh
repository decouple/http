<?hh // strict
namespace Decouple\HTTP\Router\Route;
use Decouple\Decoupler\Decoupler;
use Decouple\HTTP\Request\Request;
interface RouteInterface {
  public function execute(Request $request) : mixed;
}
