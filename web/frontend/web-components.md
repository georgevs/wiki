# Web components

### Web components interface
Definition:
```js
class CustomElement extends HTMLElement {
  // Web componenets interface ...
  
  static observedAttributes = ['attr'];

  constructor() {
    console.log('ctor');
    super();
  }
  attributeChangedCallback(name, oldVal, val) {
    console.log('attributeChanged', { name, oldVal, val });
    this.render({ [name]: val });
  }
  connectedCallback() { 
    console.log('connected');
  }
  disconnectedCallback() {
    console.log('disconnected');
  }

  // custom element interface ...

  get attr() { return this.getAttribute('attr') }
  set attr(val) { return this.setAttribute('attr', val) }

  render(options) {
    console.log('render', Object.keys(options));
  }
}

window.customElements.define('x-custom', CustomElement);
```
Usage:
```html
<body>
  <x-custom attr="123">Abc</x-custom>

  <script>
    window.addEventListener('load', () => {
      [document.createElement('x-custom')].forEach(el => {
        el.attr = '456';
        el.textContent = 'Xyz';
        document.body.appendChild(el);
      });
    });
  </script>
</body>
```

### Shadow DOM and templates
Definition:
```js
class ListElement extends HTMLElement {
  static registerCustomElement() {
    ListElement.template = document.createElement('template');
    ListElement.template.innerHTML = `
      <style>
        :host {
          display: flex;
          flex-direction: column;
        }
        .d-none {
          display: none;
        }
      </style>
      <slot></slot>
    `;

    window.customElements.define('x-list', ListElement);
  }

  constructor() {
    super();
    const shadowRoot = this.attachShadow({ mode: 'open' });
    shadowRoot.appendChild(ListElement.template.content.cloneNode(true));
  }
  
  get items() {
    return this.querySelectorAll('x-list-item');
  }
}

class ListItemElement extends HTMLElement {
  static registerCustomElement() {
    ListItemElement.template = document.createElement('template');
    ListItemElement.template.innerHTML = `
      <style>
        .d-none {
          display: none;
        }
      </style>
      <p class='label d-none'></p>
      <slot></slot>
    `;
    window.customElements.define('x-list-item', ListItemElement);
  }
  
  static observedAttributes = ['label'];
  
  constructor() {
    super();
    const shadowRoot = this.attachShadow({ mode: 'open' });
    shadowRoot.appendChild(ListItemElement.template.content.cloneNode(true));
  }

  attributeChangedCallback(name, oldVal, val) {
    this.render({ [name]: val });
  }

  get label() { return this.getAttribute('label') }
  set label(val) { this.setAttribute('label', val) }

  render({ label }) {
    if (label !== undefined) {
      [this.shadowRoot.querySelector('p.label')].forEach(p => {
        p.textContent = label.toString();
        p.classList.toggle('d-none', this.label.textContent === '');
      });
    }
  }
}

[ListElement, ListItemElement].forEach(x => { x.registerCustomElement() });
```

Usage:
```html
<body>
  <x-list>
    <x-list-item label="1">Abc</x-list-item>
    <x-list-item label="2">Def</x-list-item>
    <x-list-item>Ghi</x-list-item>
  </x-list>
</body>
```
