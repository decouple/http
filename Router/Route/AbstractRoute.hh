<?hh // strict
namespace Decouple\HTTP\Router\Route;
use Decouple\HTTP\Request\Request;
use Decouple\HTTP\Request\Uri;
abstract class AbstractRoute implements RouteInterface {

  public function __construct(public string $method, public string $pattern, public mixed $callback=null) {
    // Do nothing
  }

  abstract public function execute(Request $request) : mixed;

  public function isValid(Uri $uri) : bool {
    $remaining = preg_replace('|' . $this->pattern . '|', '', $uri);
    return strlen($remaining) == 0 || $remaining == "/";
  }

  public function getParams(Uri $uri) : Vector<string> {
    $matches = [];
    $result = Vector {};
    if($this->isValid($uri)) {
      preg_match_all('|' . $this->pattern . '|', $uri, $matches);
      if(is_array($matches)) {
        $result->addAll($this->reduce($matches));
      }
    }
    return $result;
  }

  private function reduce(KeyedTraversable<mixed,string> $arr) : Vector<string> {
    $res = Vector {};
    array_walk_recursive($arr, function($a) use ($res) { $res[] = $a; });
    return $res;
  }

  public function from(mixed $source) : AbstractRoute {
    return $this;
  }
}
