# Git Workflow & Branching Strategy

## Branch Structure

### Main Branches

- **`main`** - Production-ready releases only
  - Protected branch with required reviews
  - Only accepts merges from `develop` via release PRs
  - Tagged with version numbers (v1.0.0, v1.1.0, etc.)

- **`develop`** - Integration branch for features
  - All feature development merges here first
  - Continuous integration runs on every commit
  - Should always be in a buildable state

### Supporting Branches

#### Feature Branches
- **Pattern**: `feature/description-of-feature`
- **Examples**:
  - `feature/modeling-tools`
  - `feature/ui-improvements`
  - `feature/nurbs-surfaces`
  - `feature/plugin-system`
- **Lifetime**: Created from `develop`, merged back to `develop`

#### Bugfix Branches
- **Pattern**: `fix/issue-description`
- **Examples**:
  - `fix/viewport-rendering-crash`
  - `fix/memory-leak-mesh-loading`
- **Lifetime**: Created from `develop`, merged back to `develop`

#### Release Branches
- **Pattern**: `release/v1.0.0`
- **Purpose**: Prepare releases, bug fixes only
- **Lifetime**: Created from `develop`, merged to both `main` and `develop`

#### Hotfix Branches
- **Pattern**: `hotfix/critical-issue`
- **Purpose**: Critical fixes for production
- **Lifetime**: Created from `main`, merged to both `main` and `develop`

## Workflow Process

### 1. Feature Development
```bash
# Start new feature
git checkout develop
git pull origin develop
git checkout -b feature/my-awesome-feature

# Develop and commit
git add .
git commit -m "feat: add awesome feature"

# Push and create PR
git push -u origin feature/my-awesome-feature
# Open PR from feature/my-awesome-feature -> develop
```

### 2. Code Review Process
- All changes require code review
- At least 1 approval from core team member
- CI/CD must pass (builds + tests)
- Documentation must be updated if needed

### 3. Release Process
```bash
# Create release branch
git checkout develop
git checkout -b release/v1.1.0

# Final preparations (version updates, changelog)
git add .
git commit -m "chore: prepare v1.1.0 release"

# Merge to main
git checkout main
git merge --no-ff release/v1.1.0
git tag -a v1.1.0 -m "Release v1.1.0"

# Merge back to develop
git checkout develop
git merge --no-ff release/v1.1.0

# Push everything
git push origin main develop v1.1.0
```

## Commit Message Convention

We follow [Conventional Commits](https://www.conventionalcommits.org/):

### Format
```
type(scope): description

[optional body]

[optional footer]
```

### Types
- **feat**: New feature
- **fix**: Bug fix
- **docs**: Documentation changes
- **style**: Code style changes (formatting, etc.)
- **refactor**: Code refactoring
- **test**: Adding or updating tests
- **chore**: Build process, dependency updates
- **perf**: Performance improvements

### Examples
```bash
feat(geometry): add NURBS surface support
fix(ui): resolve viewport resize crash
docs(api): update mesh generation documentation
refactor(core): improve memory management
test(geometry): add half-edge mesh tests
chore(deps): update Qt to 6.7.0
```

## Branch Protection Rules

### `main` Branch
- Require pull request reviews (2 approvals)
- Dismiss stale reviews when new commits are pushed
- Require status checks to pass (CI/CD)
- Restrict pushes to administrators only
- Include administrators in restrictions

### `develop` Branch
- Require pull request reviews (1 approval)
- Require status checks to pass (CI/CD)
- Allow administrators to bypass requirements (for hotfixes)

## Release Strategy

### Version Numbering
We use [Semantic Versioning](https://semver.org/): `MAJOR.MINOR.PATCH`

- **MAJOR**: Breaking changes to API or major architectural changes
- **MINOR**: New features, backward compatible
- **PATCH**: Bug fixes, backward compatible

### Release Schedule
- **Major releases**: Quarterly (every 3 months)
- **Minor releases**: Monthly feature releases
- **Patch releases**: As needed for critical fixes

### Release Checklist
- [ ] All planned features merged to `develop`
- [ ] All tests passing
- [ ] Documentation updated
- [ ] CHANGELOG.md updated
- [ ] Version numbers updated in code
- [ ] Release notes prepared
- [ ] Binary artifacts built and tested

## Development Environment

### Required Tools
- Git 2.30+
- CMake 3.28+
- Qt 6.5+
- C++23 compatible compiler

### Setup
```bash
# Clone repository
git clone https://github.com/The-No-Hands-company/RudeBase3D.git
cd RudeBase3D

# Install git hooks (optional)
cp .githooks/* .git/hooks/
chmod +x .git/hooks/*
```

## Resources

- [GitHub Flow](https://guides.github.com/introduction/flow/)
- [Conventional Commits](https://www.conventionalcommits.org/)
- [Semantic Versioning](https://semver.org/)
- [Code Review Guidelines](CONTRIBUTING.md#code-review)
