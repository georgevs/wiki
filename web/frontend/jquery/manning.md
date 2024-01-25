# Manning jQuery

## Select collection
```
$(sel | el | html-text) -> collection

(coll).each(fn)
(coll).filter(fn)... end()

(coll).is(sel)
(coll).has(sel)
```

## Manipulate collection
```
(coll).append(el | coll | html-text | fn(ix, content) -> content)
(coll).prepend(...)
(coll).relaceAll(sel)
(coll).relaceWith(el | fn() -> content)

(coll).appendTo(sel | el)
(coll).prependTo(sel | el)

(coll).insertBefore(sel | el)
(coll).insertAfter(sel | el)

(coll).wrap(el)
(coll).wrapInner(el)
(coll).unwrap()
(coll).wrapAll(el)

(coll).remove(sel)
(coll).detach(sel)
(coll).empty()
```

## Manipulate items
```
(coll).attr(name) -> val
(coll).attr(name, val)
(coll).attr(name, fn(ix, val) -> val)
(coll).attr({ name: val,... })

(coll).data(name) -> val
(coll).data(name, val)
(coll).data(name, fn(ix, val) -> val)

(coll).addClass(names)
(coll).addClass(fn(ix, names) -> names)
(coll).removeClass(names)
(coll).toggleClass(names, [switch: Boolean])

(coll).css(name) -> val
(coll).css(name, val)
(coll).css(name, fn(ix, val) -> val)
(coll).css({ name: val,... })

(coll).val() -> val | [val]
(coll).val(val)
(coll).val(fn(ix, val) -> val)
```

## Events

### DOM Level 2 events propagation
- once from TOP to TARGET during CAPTURE phase
- once from TARGET to TOP during BUBBLE phase

```
(el).addEventListener(name, fn(event) -> cancel:Boolean)
```

Propagation control  
`preventDefault()` - prevents semantic with ACTION  
`stopPropagation()` - stops any further BUBBLE propagation  
`stopImmediatePropagation()`

Bind
```
(sel).bind(eventType, data, fn(event))  // data is attached to event
(sel).bind({ eventType: fn, ... })
(sel).unbind(eventType, fn)
```
jQuery can group events by NAMESPACE
```
(sel).bind('eventType.namespace.namespace...', fn)
(sel).unbind('.namespace')
```
Live
```
$(sel, ctx).live(eventType, data, fn(e))
(ctx).bind(eventType, e => $(sel).each(el => fn.call(el, e)))
```
Trigger events
```
(coll).trigger(eventType, data)
(coll).toggle(fn, ...)  // circular `click` handlers
```
Mouse events  
`mouseover` - moving over element, but not its children  
`mouseout` - moving out element, even if over its children  
`mouseenter` - once moving IN element's bounds  
`mouseleave` - once moving OUT element's bounds  
## Animations
`hide/show/toggle(speed, callback)` - show/hide  
`fadeIn/Out(speed, callback); fadeTo(speed, opacity, callback)` - fade in/out  
`slideDown/Up/Toggle(speed, callback)` - slide down/up  
`stop(clearQueue, gotoEnd)` - stop animations  
`animate(props, duration, easing, callback)` - general animation  
```
(coll).queue('fx', fn)
(coll).dequeue('fx')
(coll).delay(duration, 'fx')
```
## Functional
```
$.browser
$.extend(deep, target, src,...) -> target
$.each(coll, fn(i, val))
$.grep(ar, fn(val,i) -> T) -> ar
$.map(ar, fn(val, i) -> val) -> ar
$.inArray(val, ar)
$.makeArray(coll)
$.unique(ar) -> ar
$.merge(ar1,ar2) -> ar
$.contains(container, containee) -> T
$.data(el, name, val)
$.param(params)...
```
## Extending jQuery
```
$.mynamespace.myfn = function(...) {...}
$.fn.myfn = function(...) { const coll = this; ... return this; } // chain coll
$(coll).myfun(...).nextinchain(...)

IDIOM:
$.fn.myfun = function() {
  return this.each(function() { $(this).dosomething() })
  return this.css('color', fn() { return $(this)...? value... })
}
```
## Plugin
```
(function ($) {  // iife
  $.fn.myfn = function (options) {
    const params = $.extend({ ...defaults }, options || {})
    ... init; hook events; ...
    return this; // chain coll
  }
}(jQuery))
```
## XHR (XMLHttpRequest)
```
xhr = new XMLHttpRequest()
xhr.open('GET', '/some/resource')
xhr.onreadystatechange = function () {
  this.readystate DONE
  this.status 200
  this.responseText
}
shr.send(body || null)
```
## Ajax
```
const param = '...' // GET request
const param = {...} // POST request
$(coll).load(url, param, fn(response, status, xhr))
$.get(url, params, fn(response, status, xhr), type) // type = html|text|xml|json
$.post(url, params, fn(response, status, xhr), type)
$.getJSON(url, params, fn(response, status, xhr))
$.ajax({ url, type, data, dataType, async,  // async = true|false
         success: fn, error: fn, complete: fn, beforeSend: fn, dataFilter: fn })
```
