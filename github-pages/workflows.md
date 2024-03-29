# Build and deploy to GitHub pages with workflows

Sample `hello-world/.github/workflows/app-gh-pages.yml` workflow to deploy a project folder in GitHub pages with a MANUAL trigger

```yml
name: Deploy to GitHub Pages

on:
  workflow_dispatch:

permissions:
  contents: read
  pages: write
  id-token: write

concurrency:
  group: "pages"
  cancel-in-progress: false

jobs:
  build:
    runs-on: ubuntu-latest
    steps:
      - name: Checkout
        uses: actions/checkout@v3
      - name: Upload artifact
        uses: actions/upload-pages-artifact@v2
        with:
          path: app/

  deploy:
    environment:
      name: github-pages
      url: ${{ steps.deployment.outputs.page_url }}
    runs-on: ubuntu-latest
    needs: build
    steps:
      - name: Deploy to GitHub Pages
        id: deployment
        uses: actions/deploy-pages@v2
```
