<?hh // strict
namespace Decouple\HTTP\Request;
class Uri {
  private Vector<string> $parts;
  private string $uri;
  public function __construct(string $uri) {
    $this->uri = $uri;
    $this->parts = Vector {};
    $parts = explode('/', $uri);
    foreach($parts as $part) {
      if(!is_null($part) && strlen($part) !== 0) {
        $this->parts->add($part);
      }
    }
  }
  public function __toString() : string {
    return $this->uri;
  }
  public function parts() : Vector<string> {
    return $this->parts;
  }
}
