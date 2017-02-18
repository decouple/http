<?hh // strict
namespace Decouple\HTTP\Router\Route;
use Decouple\Decoupler\Decoupler;
use Decouple\HTTP\Request\Request;
use Decouple\HTTP\Request\Uri;
interface RouteInterface {
  public function execute(Request $request) : mixed;
  public function isValid(Uri $uri) : bool;
  public function getParams(Uri $uri) : Vector<string>;
}
