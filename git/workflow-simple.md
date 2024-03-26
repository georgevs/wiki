# Git simplified workflow

This simplified workflow prioritizes always working in `master`, 
and is very practical when working ALONE in a MONOREPO.  
The workflow rests on the premise that it's not practical, hence advisable, 
same person to work on multiple features of the SAME project simultaneously.  
Yet, experimentation is still possible, but tracked through tags.

### Approach
Each host has a local `HOST/master` branch.  
Saving experimental work is local through `HOST/PROJ/FEAT/DATE` tag with a message.  
Switching projects while work-in-progress is local through `HOST/wip/PROJ` branch.  
Transferring work on a different host is by pushing and rebasing onto `origin/wip/PROJ` branch.  
Publishing work from `HOST/master` is by rebasing and pushing to `origin/master`.  


### Start work on host
```bash
git fetch origin
git checkout -b HOST/master origin/master
```

### Resume work on host
The simplified approach assumes ALWAYS working in `HOST/master`.  
It's a good practice to keep local `HOST/master` up-to-date, 
hence it's advisable to rebase onto fresh `origin/master` BEFORE starting new changes.
```bash
git fetch origin
git rebase origin/master
```

### Save experimental work on host
Experimental work is saved through tagging in `HOST/master`.  
`HOST/master` branch can then be reset to any prior base.  
NOTE: no need to add message or details other than date to the tag,
since the commit message should be descriptive enough.
```bash
git tag HOST/PROJ/FEAT/DATE
git tag --list -n
git reset --hard origin/master
```

### Switch projects on host
NOTE: The capital `-B` option will RESET an already existing branch.
```bash
# commit work in progress
git checkout -B HOST/wip/PROJ
git add .
git commit -m 'wip: ...'

# continue with unrelated work
git checkout HOST/master

# resume work in progress
git checkout HOST/wip/PROJ
git rebase HOST/master
git checkout -B HOST/master HOST/wip/PROJ
```

### Transfer work in progress to another host
```bash
# push work in progress from host A
git push --force origin HOST/wip/PROJ:wip/PROJ

# resume work in progress on host B
git fetch origin
git checkout HOST/wip/PROJ
git rebase origin/wip/PROJ
git rebase HOST/master
git checkout -B HOST/master HOST/wip/PROJ
```

### Publish work
```bash
git fetch origin
git rebase origin/master
git push origin HOST/master:master
```
