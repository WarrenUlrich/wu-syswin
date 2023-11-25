with import <nixpkgs> {};

pkgs.mkShell rec {
  nativeBuildInputs = [ cmake pkg-config ];

  buildInputs = [
    xorg.libX11 xorg.libxcb
  ];
}