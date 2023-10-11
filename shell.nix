{ pkgs ? import <nixpkgs> {} }:

pkgs.llvmPackages_16.stdenv.mkDerivation {
  name = "godot-nix-shell";
  buildInputs = with pkgs; [
    catch2_3
    clang_16
    clang-tools_16
    cmake
    godot_4
    ninja
    python311
  ];
  shellHook = ''
    export CMAKE_GENERATOR=Ninja
  '';
}

