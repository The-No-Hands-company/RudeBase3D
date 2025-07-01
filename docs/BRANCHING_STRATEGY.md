# Branching Strategy

## 🌳 Git Workflow Model

RudeBase3D uses a **Git Flow** inspired branching model optimized for open source development with continuous integration and regular releases.

## 📋 Branch Types

### **Main Branches**

#### `main` 
- **Purpose**: Production-ready code
- **Protection**: Protected branch, no direct commits
- **Merges**: Only from `develop` via pull request
- **Versioning**: All commits tagged with release versions
- **CI/CD**: Triggers production builds and releases

#### `develop`
- **Purpose**: Integration branch for features
- **Protection**: Protected branch, requires reviews
- **Merges**: Feature branches, hotfixes, release preparations
- **CI/CD**: Continuous testing and preview builds
- **Stability**: Should always be in buildable state

### **Supporting Branches**

#### Feature Branches (`feature/*`)
- **Naming**: `feature/short-description` or `feature/issue-number-description`
- **Source**: Branch from `develop`
- **Merge**: Back to `develop` via pull request
- **Lifetime**: Short-lived, deleted after merge
- **Examples**:
  - `feature/half-edge-subdivision`
  - `feature/material-editor-ui`
  - `feature/obj-file-import`

#### Release Branches (`release/*`)
- **Naming**: `release/v1.2.0`
- **Source**: Branch from `develop`
- **Purpose**: Prepare new production release
- **Merge**: To both `main` and `develop`
- **Lifetime**: Until release is complete
- **Activities**: Bug fixes, documentation updates, version bumps

#### Hotfix Branches (`hotfix/*`)
- **Naming**: `hotfix/v1.2.1-critical-bug`
- **Source**: Branch from `main`
- **Purpose**: Critical production fixes
- **Merge**: To both `main` and `develop`
- **Lifetime**: Very short, immediate fix cycle

#### Experimental Branches (`experiment/*`)
- **Naming**: `experiment/description`
- **Source**: Usually from `develop`
- **Purpose**: Research, proof-of-concepts, major refactoring
- **Merge**: May or may not be merged
- **Lifetime**: Variable, often long-lived

## 🔄 Workflow Process

### **Feature Development**
```bash
# Start new feature
git checkout develop
git pull origin develop
git checkout -b feature/my-new-feature

# Develop feature
git add .
git commit -m "feat: implement feature X"
git push -u origin feature/my-new-feature

# Submit pull request to develop
# After review and approval, merge and delete branch
```

### **Release Process**
```bash
# Create release branch
git checkout develop
git checkout -b release/v1.2.0

# Prepare release (version bumps, changelog, docs)
git commit -m "chore: prepare v1.2.0 release"
git push -u origin release/v1.2.0

# After testing, merge to main and develop
# Tag the release on main
git tag v1.2.0
```

### **Hotfix Process**
```bash
# Critical bug found in production
git checkout main
git checkout -b hotfix/v1.2.1-security-fix

# Fix the issue
git commit -m "fix: resolve security vulnerability"
git push -u origin hotfix/v1.2.1-security-fix

# Merge to main and develop, tag new version
```

## 📊 Branch Protection Rules

### **Main Branch**
- ✅ Require pull request reviews (2 approvals)
- ✅ Dismiss stale reviews when new commits pushed
- ✅ Require status checks to pass
- ✅ Require branches to be up to date
- ✅ Require conversation resolution before merging
- ✅ Restrict pushes that create files larger than 100MB
- ✅ Require signed commits

### **Develop Branch**
- ✅ Require pull request reviews (1 approval)
- ✅ Require status checks to pass
- ✅ Require branches to be up to date
- ✅ Allow force pushes by administrators only

## 🏷️ Tagging Strategy

### **Version Tags**
- **Format**: `v{MAJOR}.{MINOR}.{PATCH}` (e.g., `v1.2.3`)
- **Location**: Only on `main` branch
- **Timing**: Immediately after release merge
- **Annotation**: Include release notes summary

### **Pre-release Tags**
- **Alpha**: `v1.2.0-alpha.1` (early development)
- **Beta**: `v1.2.0-beta.1` (feature complete, testing)
- **RC**: `v1.2.0-rc.1` (release candidate)

### **Special Tags**
- **Milestones**: `milestone-1.0` (major project milestones)
- **Archive**: `archive-old-system` (preserve important states)

## 🔍 Commit Message Convention

Following [Conventional Commits](https://www.conventionalcommits.org/):

### **Format**
```
<type>[optional scope]: <description>

[optional body]

[optional footer(s)]
```

### **Types**
- **feat**: New feature
- **fix**: Bug fix
- **docs**: Documentation changes
- **style**: Code style changes (formatting, etc.)
- **refactor**: Code refactoring
- **perf**: Performance improvements
- **test**: Test additions or modifications
- **chore**: Build process or auxiliary tool changes
- **ci**: CI/CD changes

### **Scopes**
- **core**: Core engine systems
- **geometry**: Geometry processing
- **rendering**: Rendering pipeline
- **ui**: User interface
- **tools**: Modeling tools
- **io**: Input/output systems
- **docs**: Documentation
- **build**: Build system

### **Examples**
```
feat(geometry): add half-edge mesh subdivision
fix(rendering): resolve OpenGL context issues
docs(api): update Transform class documentation
refactor(core): improve memory pool implementation
```

## 🚀 Release Management

### **Release Schedule**
- **Major Releases**: Every 6 months (January, July)
- **Minor Releases**: Monthly on 1st Tuesday
- **Patch Releases**: As needed for critical issues
- **Pre-releases**: 2 weeks before major/minor releases

### **Release Checklist**
- [ ] All features complete and tested
- [ ] Documentation updated
- [ ] Changelog generated
- [ ] Version numbers bumped
- [ ] CI/CD passes all tests
- [ ] Security review completed
- [ ] Performance benchmarks acceptable
- [ ] Release notes prepared

## 🔧 Branch Maintenance

### **Regular Cleanup**
- **Weekly**: Review and merge ready feature branches
- **Monthly**: Clean up merged branches
- **Quarterly**: Review experimental branches
- **Annually**: Archive old release branches

### **Automated Actions**
- **Stale Branch Detection**: Mark branches inactive for 30+ days
- **Auto-Delete**: Remove merged feature branches after 7 days
- **Dependency Updates**: Weekly automated dependency PRs
- **Security Scans**: Daily security vulnerability checks

## 📈 Metrics and Monitoring

### **Branch Health Metrics**
- **Feature Branch Lifetime**: Target < 2 weeks
- **Pull Request Size**: Target < 500 lines changed
- **Review Time**: Target < 48 hours
- **Build Success Rate**: Target > 95%

### **Quality Gates**
- **Code Coverage**: Minimum 80%
- **Performance Tests**: No regression > 5%
- **Security Scans**: Zero high-severity issues
- **Documentation**: All public APIs documented

## 🛠️ Tools and Integration

### **GitHub Features**
- **Actions**: Automated CI/CD pipelines
- **Discussions**: Feature planning and community input
- **Projects**: Milestone and sprint planning
- **Security**: Dependency scanning and alerts

### **External Tools**
- **CodeClimate**: Code quality analysis
- **Codecov**: Test coverage reporting
- **Snyk**: Security vulnerability scanning
- **SonarCloud**: Static code analysis

## 📚 Training and Documentation

### **New Contributor Guide**
1. **Setup**: Local development environment
2. **Workflow**: Branch creation and PR process
3. **Standards**: Coding and commit conventions
4. **Testing**: Running and writing tests
5. **Review**: Code review process and expectations

### **Best Practices**
- **Small Commits**: Atomic, focused changes
- **Clear Messages**: Descriptive commit messages
- **Regular Rebasing**: Keep feature branches current
- **Clean History**: Squash commits before merging
- **Documentation**: Update docs with code changes

## 🎯 Future Improvements

### **Planned Enhancements**
- **Automated Release Notes**: Generate from commit messages
- **Semantic Release**: Automated version bumping
- **Multi-Repository**: Coordinate across related projects
- **Advanced Merging**: Investigate alternative merge strategies

### **Community Feedback**
Regular surveys and discussions to refine the branching strategy based on contributor experience and project needs.

---

*This branching strategy will evolve with the project's maturity and team size.*
