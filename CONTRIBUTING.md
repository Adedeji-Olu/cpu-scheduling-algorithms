# Contributing Guidelines

## Team Workflow

This project uses a collaborative Git workflow with three team members:
- **Adedeji Olu** - Project Lead
- **Almamy Sow** - Testing & Documentation  
- **Brionna Nunn** - Performance Analysis

## Git Workflow

### Branching Strategy

- `master` - Main production branch
- `feature/*` - New features
- `bugfix/*` - Bug fixes
- `docs/*` - Documentation updates

### Making Changes

1. **Create a branch:**
```bash
   git checkout -b feature/your-feature-name
```

2. **Make your changes and commit:**
```bash
   git add .
   git commit -m "type: description"
```

3. **Push to GitHub:**
```bash
   git push origin feature/your-feature-name
```

4. **Create a Pull Request** on GitHub

### Commit Message Format
```
type: brief description

Detailed explanation (optional)
```

**Types:**
- `feat`: New feature
- `fix`: Bug fix
- `docs`: Documentation changes
- `test`: Adding/updating tests
- `refactor`: Code refactoring
- `chore`: Build/config changes

### Examples
```bash
git commit -m "feat: Add starvation prevention to priority scheduler"
git commit -m "test: Add unit tests for MLFQ demotion logic"
git commit -m "docs: Update performance analysis with benchmark results"
```

## Code Standards

- Follow existing code style
- Add comments for complex logic
- Write tests for new features
- Update documentation

## Testing

All changes must pass:
```bash
make test
```

Before submitting a PR!
