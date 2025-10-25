with import <nixpkgs> {};
let
  # Use unwrapped clang to avoid hardening flags
  unwrappedClang = clang_multi.cc;
in
mkShell {
  nativeBuildInputs = [
    gnumake
    unwrappedClang
    llvm
  ];

  # Override CC to use unwrapped clang (avoids Nix hardening wrapper)
  # Set resource directory for built-in headers (stddef.h, stdint.h, etc.)
  shellHook = ''
    export EXTRA_CFLAGS="-resource-dir ${unwrappedClang.lib}/lib/clang/19"
  '';
}
