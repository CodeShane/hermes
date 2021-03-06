hermes-pkgstore(1)
==================

## SYNOPSIS

`hermes-pkgstore init ...`<br>
`hermes-pkgstore build ...`<br>
`hermes-pkgstore gc ...`<br>
`hermes-pkgstore send ...`<br>
`hermes-pkgstore recv ...`<br>
`hermes cp ...`<br>
`hermes version ...`<br>

## DESCRIPTION

The `hermes-pkgstore` command is used to manage hermes package stores.
Most users should use hermes(1) instead of directly interacting with `hermes-pkgstore`.

When `hermes-pkgstore` is installed with setuid+setgid root, it allows less privileged
users to perform operations on the global package store if they are authorized
by the hermes-package-store(7) config.

The `hermes-pkgstore` command is split into sub-commands which can be executed to interact with the package store. Each subcommand listed in the synopsis has its own man page,
which are listed in the next section.

## SUBCOMMANDS

* hermes-pkgstore-init(1) - Initialize a package store.
* hermes-pkgstore-build(1) - Build a package thunk generated by hermes(1).
* hermes-pkgstore-gc(1) - Remove packages that are no longer in use.
* hermes-pkgstore-send(1) - Send a signed package and its dependencies over stdin/stdout.
* hermes-pkgstore-recv(1) - Receive a signed package and its dependencies over stdin/stdout.
* hermes-pkgstore-version(1) - Print the version.

## SEE ALSO

hermes(1), hermes-package-store(7)
