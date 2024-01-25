# GitHub Pages

[GitHub: Pages](https://pages.github.com)  
[Build and deploy with workflows](./workflows.md)  

## Summary
GitHub can serve a static website built from a REPO (aka GitHub Pages).

The website is served at https://USERNAME.github.io/REPO by default, or can be served at a custom domain (naked and/or subdomain) if configured from the REPO's pages settings.

The REPO can be configured to build and deploy the website either:
- AUTOMATICALLY, whenever a commit places a change into a BRANCH/FOLDER, or
- through a CUSTOM GitHub actions flow.


## Build and deploy automatically
Setup REPO's page settings at https://github.com/USERNAME/REPO/settings/pages to build and deploy the website whenever a commit places a change in REPO's BRANCH/FOLDER.

Served from https://USERNAME.github.io:
```
git push git@github.com:USERNAME/USERNAME.github.io.git HEAD:main
curl https://USERNAME.github.io
```

Served from https://USERNAME.github.io/REPO:
```
git push git@github.com:USERNAME/REPO.git HEAD:gh-pages
curl https://USERNAME.github.io
```

### Details
Automatic build uses Jekyll. Add `.nojekyll` empty file to deployment if necessary.

CI/CD can be setup by configuring the REPO.

The REPO pages branch (by default `gh-pages`), a custom domain, HTTPS access etc can be configured from the REPO pages settings at https://github.com/USERNAME/REPO/settings/pages.

A custom domain may be VERIFIED from the USER account pages settings at https://github.com/settings/pages.


## Custom domain for website served by GitHub pages
Steps:
1. Configure the custom domain provider DNS to point to GitHub pages servers.
2. Configure the GitHub repo pages settings to serve the custom domain.
3. (Optional,Recommended) In GitHub verify the custom domain.

### Configure the custom domain provider
In the domain provider (https://namecheap.com) add the following records for the custom domain:
```
    A record:   @ -> 185.199.108.153
    A record:   @ -> 185.199.109.153
    A record:   @ -> 185.199.110.153
    A record:   @ -> 185.199.111.153
CNAME record: www -> USERNAME.github.io
```
Check the domain configuration:
```
dig DOMAIN.COM +noall +answer -t A
dig www.DOMAIN.COM +nostats +nocomments +nocmd
```
### Configure the GitHub repo pages settings
In the GitHub repo pages settings at https://github.com/USERNAME/REPO/settings/pages set the custom domain to DOMAIN.COM.  GitHub will automatically check the domain, and request a TLS certificate from Let's encrypt.

Enable HTTPS, to instruct GitHub to serve secure only. It may take some time to acquire the TLS certificate from Let's encrypt.

Once all is complete, the page settings will indicate the serve url as https://DOMAIN.COM.

### Verify the custom domain
Verifying a custom domain within a GitHub account prevents serving spoof websites of that domain from other GitHub users.

To verify a custom domain, add the custom domain in the USER account pages settings, and follow the DNS TXT challenge instructions to successful completion.
