# Hermes

Hermes is a software environment manager.

Hermes may be considered similar to a source package manager, but certainly
works quite differently from traditional tools in this space.
You could imagine it doing jobs similar to software like [homebrew](https://brew.sh/)
or perhaps docker, but in a totally different way. For those who know
[Nix](https://nixos.org/) and [Guix](https://guix.gnu.org/), things will be familiar, but
there are also some key differences.

## So what does it do?

When using Hermes your *entire* software stack is defined and saved in a set of
package definitions along side the rest of your source code. This means Hermes
gives you strict reproducible control over your entire software stack. With
a single command you can reliably (and quickly) rebuild a development environment,
deployment, docker image or vm image *entirely* from source.

With Hermes you can also share, review, clone, fork, patch and pin your whole software stack
the same way you do with your existing source code.

Hermes also provides some nifty features:

- Transparent build caching.
- Remote package builds.
- Remote deployment.
- Atomic upgrades and rollback.

## Does this replace my language package manager like npm,jpm,cargo,pip... ?

No.

Hermes can be used to manage the system level dependencies during software
development while still using these tools normally. For example you could install
a specific C compiler and libcurl with hermes, but just invoke npm as normal for your
javascript project.

During deployment of your software you can invoke these tools from within
package definitions to build your software in a well defined sandbox.
These packages can then be deployed to remote machines using 'hermes cp'.

## Does this replace my system package manager?

Maybe.

Hermes packages are totally separate from your system packages. When you enter
a hermes environment, your PATH and other environment variables are set to paths
within the hermes package store.

Hermes does not care what other package managers you use, and does not install
packages in the traditional ```/bin``` path so will not conflict with them.

## What does using hermes look like?

Coming soon...