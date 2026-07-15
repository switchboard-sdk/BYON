from invoke import task, Context

import os

PROJECT_DIR = os.path.dirname(os.path.abspath(__file__))

@task
def setup(ctx):
    """Set up ExampleDSP Extension"""
    pass

@task
def clean(ctx):
    """Clean ExampleDSP Extension"""
    pass

@task
def build_macos(ctx, clean=False, build_type="Release"):
    """Build the macOS version of the ExampleDSP Extension"""
    if clean:
        ctx.run("rm -rf build")
    ctx.run(
        f"cmake -B build -G Xcode . &&"
        f"cmake --build build --config {build_type} &&"
        f"cmake --install build --config {build_type} --prefix out"
    )

@task
def build_linux(ctx, clean=False, build_type="Release"):
    """Build the Linux version of the ExampleDSP Extension"""
    if clean:
        ctx.run("rm -rf build")
    ctx.run(
        f"cmake -B build . &&"
        f"cmake --build build --config {build_type} &&"
        f"cmake --install build --config {build_type} --prefix out"
    )

@task
def build_windows(ctx, clean=False, build_type="Release"):
    """Build the Windows version of the ExampleDSP Extension"""
    if clean:
        ctx.run("rm -rf build")
    ctx.run(
        f"cmake -B build . &&"
        f"cmake --build build --config {build_type} &&"
        f"cmake --install build --config {build_type} --prefix out"
    )
