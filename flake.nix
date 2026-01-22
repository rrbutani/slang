{
    inputs.nixpkgs.url = "github:NixOS/nixpkgs/nixpkgs-unstable";
    inputs.flake-utils.url = "github:numtide/flake-utils";

    outputs = inputs: inputs.flake-utils.lib.eachDefaultSystem (system: let
        pkgs = inputs.nixpkgs.legacyPackages.${system};

        np = pkgs.pkgsStatic;
    in {
        devShells.default = np.mkShell {
            nativeBuildInputs = with np; [
                cmake ninja clang-tools python3
            ];
            buildInputs = with np; [
                mimalloc boost182 fmt_11 catch2_3
                # python3Packages.pybind11
            ];
        };
    });
}
