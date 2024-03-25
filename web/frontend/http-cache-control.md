# HTTP cache control

[MDN: HTTP caching](https://developer.mozilla.org/en-US/docs/Web/HTTP/Caching)  
[Jake Archibald: Caching best practices](https://jakearchibald.com/2016/caching-best-practices/)  
[Harry Roberts: Cache Rules Everything](https://www.youtube.com/watch?v=QZo6L6IZm2o)  
[Everything you need to know about HTTP Caching](https://www.youtube.com/watch?v=HiBDZgTNpXY)  


### Types of cache
BROWSER cache - local in the browser  
INTERMEDIARY cache - remote in ISP, CDN, proxy, etc

### Terminology
CACHE - how long can I use X without revalidate (aka server interaction)  
REVALIDATE - how do I check if X changed AFTER cache expired  
FRESH - X is in cache, and NOT cache expired  
STALE - X is in cache, but IS cache expired  

### HTTP cache headers
CACHE headers - cache-control, ...
REVALIDATE headers - etag, if-none-match, ...

DEPRECATED:
```
expires: Fri, 30 Oct 2020 21:07:12 GMT     <-- absolute time
pragma: no-cache  <-- preload cache
```

PREFERRED:
```
cache-control:
  max-age   - cache timeout in seconds
  s-max-age - cache timeout in seconds for intermediaries
  no-store  - do not store cache
  no-cache  - use cache only AFTER revalidation (always ask server, only download if new)
  private   - cache for this client ONLY (aka sensitive response)
  public    - share cache allowed (assets in CDN etc)
  immutable - will never change
  must-revalidate - REQUIRE revalidate after expiry; fail if offline after expiry 
  proxy-revalidate
  stale-while-revalidate - allow expired cache, while revalidate/fetching new
  
age:
  <secs> - time spent in intermediary (deducted from max-age for proper expiry limit)

etag | if-none-match | last-modified | if-modified-since   <-- validators

etag:
  "<hash>"    - STRONG, assume hash contents
  w/"<hash>"  - WEAK, assume hash metadata
```

### Curl show headers
```
curl -I https://...
```

### ETag/If-none-match
```
SERVER (fetch):
  cache-control: public, max-age=3600
  etag: "abc123xyz"

CLIENT (revalidate):
  if-none-match: "abc123xyz"
SERVER (revalidate, not changed):
  HTTP/1 304 Not Modified
SERVER (revalidate, changed):
  HTTP/1 200 OK
  ...data...
```

### Recommended strategy
`cache-control: private, ...` for sensitive response;  
`cache-control: public, ...` for shared assets to cache in CDN;  
`cache-control: no-store` for responses which do not make sense to cache;  
`app.<hash>.js` and `cache-control: max-age=<MAXINT>, immutable` for assets which won't ever change;  
`index.html` and `cache-control: no-cache; etag: "<hash>"` for resources likely to change ALWAYS validate, use cache if not changed, else refetch
`cache-control: max-age=<secs>` cache for reasonable time, then attempt revalidate
`cache-control: max-age=<secs>, no-cache` cache for reasonable time, then revalidate
`cache-control: max-age=<secs>, must-revalidate` fail if cannot revalidate (offline)
