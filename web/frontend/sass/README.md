# SASS

## Import
```
<style type="text/scss"> ...
```
## Partial SASS
Sass files beginning with underscore
```
@import 'name'     <--- imports `_name.sass`
```
## Extensions (like inheritance)
```
.panel { ... }
.big-panel { @extend .panel; ... }
```
## Variables
```
$main-font: Arial, sans-serif;
h1 { font-family: $main-font }
```
## Mixins (like functions)
```
@mixin box-shadow ($x, $y, $blur, $c) { ... }

div { @include box-shadow (0px, 0px, 4px, #fff); ...}
```
## Conditional
```
@if $val == light { border: 1px solid black; }
@else if $val == medium { ... }
@else { ... }

@for $i from 1 to 5 { font-#{$i}: { font-wifth: $i * 100; } }
@for $i from 1 through 5 { ... }

@each $color in red, green, blue { ... }    <--- list

$colors: (color1: red, color2: green, color3: blue);
@each $key, $color in $colors { ... }    <--- map

$i: 0; 
while $i < 5 { ...; $i: $i + 1; }
```
