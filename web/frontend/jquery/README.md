# jQuery

## Reference
- [cdnjs vs. unpkg](https://stackoverflow.com/questions/59182642/how-to-choose-a-cdn-to-load-javascript-css-libraries)
- [jQuery vs D3](https://webkid.io/blog/replacing-jquery-with-d3/)

## jQuery CDN
```
<script src="https://code.jquery.com/jquery-3.6.0.min.js" integrity="sha256-/xUj+3OJU5yExlq6GSYGSHk7tPXikynS7ogEvDej/m4=" crossorigin="anonymous"></script>
```
## Animate CDN
```
<link rel="stylesheet" href="https://cdnjs.cloudflare.com/ajax/libs/animate.css/4.1.1/animate.min.css" integrity="sha512-c42qTSw/wPZ3/5LBzD+Bw5f7bSF2oxou6wEb+I/lqeaKV5FDIfMvvRp772y4jcJLKuGUOpbJMdg/BTl50fJYAw==" crossorigin="anonymous" referrerpolicy="no-referrer" />
```
## Ready event
```
$(document).ready(function () { ... })
```
## Selector
```
$('form .btn:nth-child(2)')...   <--- `sel` can be type, class, etc.
```
## Basic functionality
```
$(sel).addClass('animated bounce shake fadeOut ...')
$(sel).css('key', 'value')
$(sel).prop('key', 'value')
$(sel).html(...)
$(sel).text(...)

$(sel).remove()
$(sel).appendTo(sel2)

$(sel).parent()
$(sel).children()
```