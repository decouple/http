<?hh // strict
namespace Decouple\HTTP\Request;
class Request {
  public Vector<string> $routeParams;
  public function __construct(
    public Uri $uri, 
    protected Map<string,mixed> $get, 
    protected Map<string,mixed> $post, 
    protected Map<string,mixed> $files
  ) {
    $this->routeParams = Vector {};
  }
  public function getParam(string $param) : mixed {
    if(!$this->get->contains($param)) {
      return false;
    }
    return $this->get->at($param);
  }
  public function postParam(string $param) : mixed {
    if(!$this->post->contains($param)) {
      return false;
    }
    return $this->post->at($param);
  }
}
