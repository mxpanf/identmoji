# Contributing to Project

First off, thanks for taking the time to contribute! ❤️

All types of contributions are encouraged and valued. See the [Table of Contents](#table-of-contents) for different ways to help and details about how this project handles them. Please make sure to read the relevant section before making your contribution.

## Table of Contents

- [Code of Conduct](#code-of-conduct)
- [I Have a Question](#i-have-a-question)
- [I Want To Contribute](#i-want-to-contribute)
  - [Reporting Bugs](#reporting-bugs)
  - [Suggesting Enhancements](#suggesting-enhancements)
  - [Your First Code Contribution](#your-first-code-contribution)
- [Pull Request Process](#pull-request-process)
- [Commit Messages](#commit-messages)
- [Styleguides](#styleguides)

## Code of Conduct

This project and everyone participating in it is governed by the
[Contributor Covenant Code of Conduct](CODE_OF_CONDUCT.md).
By participating, you are expected to uphold this code. Please report unacceptable behavior to [contrib@z3r.io](mailto:contrib@z3r.io?subject=Project:%20Identmoji%20by%20mxpanf).

## I Have a Question

If you want to ask a question, we assume that you have read the available [Documentation](docs/).

Before you ask a question, it is best to search for existing [Issues](https://github.com/mxpanf/identmoji/issues) that might help you. In case you have found a suitable issue and still need clarification, you can write your question in this issue. It is also advisable to search the internet for answers first.

If you then still feel the need to ask a question and need clarification, we recommend the following:

- Open an [Issue](https://github.com/mxpanf/identmoji/issues/new).
- Provide as much context as you can about what you're running into.
- Provide project and platform versions (nodejs, npm, etc), depending on what seems relevant.

## I Want To Contribute

### Reporting Bugs

Before creating bug reports, please check the existing issues list as you might find out that you don't need to create one. When you are creating a bug report, please include as many details as possible.

**Standard Bug Report:**
1.  **Use a clear and descriptive title** for the issue to identify the problem.
2.  **Describe the exact steps to reproduce the problem** in as many details as possible.
3.  **Describe the behavior you observed after following the steps** and point out what exactly is the problem with that behavior.
4.  **Explain which behavior you expected to see instead and why.**
5.  **Include screenshots** which show you following the reproduction steps.

### Suggesting Enhancements

This section guides you through submitting an enhancement suggestion, including completely new features and minor improvements to existing functionality.

**Standard Feature Request:**
1.  **Use a clear and descriptive title** for the issue to identify the suggestion.
2.  **Provide a step-by-step description of the suggested enhancement** in as many details as possible.
3.  **Explain why this enhancement would be useful** to most users.

### Your First Code Contribution

1.  Fork the repository.
2.  Clone your fork locally.
3.  Create a new branch for your feature or fix.
    ```bash
    git checkout -b feature/amazing-feature
    ```
4.  Make your changes.
5.  Run tests to ensure nothing is broken.

## Pull Request Process

1.  Ensure any install or build dependencies are removed before the end of the layer when doing a build.
2.  Update the `README.md` or documentation in `docs/` with details of changes to the interface, this includes new environment variables, exposed ports, useful file locations and container parameters.
3.  Increase the version numbers in any examples files and the README.md to the new version that this Pull Request would represent.
4.  You may merge the Pull Request in once you have the sign-off of two other developers, or if you do not have permission to do that, you may request the second reviewer to merge it for you.

## Commit Messages

We use **[Conventional Commits](https://www.conventionalcommits.org/)** for our commit messages. This allows us to automatically generate changelogs and semantic version numbers.

**Format:** `<type>(<scope>): <subject>`

**Types:**
* `feat`: A new feature
* `fix`: A bug fix
* `docs`: Documentation only changes
* `style`: Changes that do not affect the meaning of the code (white-space, formatting, missing semi-colons, etc)
* `refactor`: A code change that neither fixes a bug nor adds a feature
* `perf`: A code change that improves performance
* `test`: Adding missing tests or correcting existing tests
* `chore`: Changes to the build process or auxiliary tools and libraries such as documentation generation

**Example:**
```text
feat(auth): add google oauth2 login support
````

## Styleguides

### Git Flow

  * **Main Branch**: `main` (Production-ready code).
  * **Feature Branches**: Create branches from `main` using the naming convention `feature/name-of-feature` or `fix/issue-description`.

### Coding Style

  * We use `.editorconfig` to maintain consistent coding styles. Please ensure your editor supports it.
  * Formatting rules are enforced by CI. Run the linter locally before pushing.

-----

Thank you for contributing!
