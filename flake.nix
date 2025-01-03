{
  description = "Generic flake for C++ projects";

  inputs = {
    nixpkgs.url = "github:NixOS/nixpkgs/nixpkgs-unstable";
    flakelight.url = "github:nix-community/flakelight";
  };

  outputs = { flakelight, nixpkgs, ... }:
    flakelight ./. {

      inputs.nixpkgs = nixpkgs;
      nixpkgs.config = { allowUnfree = true; };

      devShell = pkgs: {

        stdenv = pkgs.llvmPackages_19.stdenv;

        packages = with pkgs; [

          spdlog

          pre-commit
          commitizen

          pkg-config
          coreutils
          clang-tools_17
          cpplint
          cmake
          cppcheck
          doxygen
          gtest
          gdb

          include-what-you-use

          # Apis & crow
          ngrok
          openssl
          asio

          # raylib # nixpkgs-unstable version is outdated
          xorg.libX11
          xorg.libXrandr
          xorg.libXinerama
          xorg.libXcursor
          glfw
          libGLU

          xorg.libXi
          wayland-scanner

          (pkgs.python312.withPackages (python-pkgs: [ ]))

        ];

      };
    };

}
