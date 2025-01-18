# HowTo

### Add to other projects

```bash
git submodule add git@github.com:Gosstik/toolbox.git
```


### Before each commit

```bash
git submodule update --init --remote --recursive --rebase
```


### Set specific branch to track

```bash
git config -f .gitmodules submodule.toolbox.branch master
git config status.submodulesummary 1
```
