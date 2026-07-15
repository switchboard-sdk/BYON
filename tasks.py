from invoke import task


@task
def rename(ctx, name=None, dest=None):
    """Generate a new project from this template (wraps scripts/rename.sh)"""
    args = " ".join(a for a in (name, dest) if a)
    ctx.run(f"scripts/rename.sh {args}".strip(), pty=True)
