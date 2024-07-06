# The maximum length car sequencing problem

This repository contains the code for the Max-CSP model introduced by [Pontes et al. (2024)](https://www.sciencedirect.com/science/article/pii/S0377221724001322).

## Running the model in Linux

The code is implemented in C++. To run the Max-CSP formulation $F1$, open the terminal and type the following commands:

```console
foo@bar:~$ make all
foo@bar:~$ ./max-csp.exe <path to instance file> -<flags>
```

Flags are available to change the selected model or bounds.

### Flags

- `binsearch`: runs model $I_B$, which performs a binary search in the feasibility version of the problem (combine with the `heuristic` flag to use the heuristic lower bound);
- `branch`: selects formulation $F2$, which branches on the first unnoccupied position;
- `decsearch`: runs model $I_D$, which performs a decremental search in the feasibility version of the problem (combine with the `heuristic` flag to use the heuristic lower bound);
- `heuristic`: runs default $F1$ model with the heuristic initialization (combine with the `noexact` flag to run only the heuristic algorithm);
- `incsearch`: runs model $I_I$, which performs an incremental search in the feasibility version of the problem (combine with the `heuristic` flag to use the heuristic lower bound);
- `minpacedelay`: runs the Slow-CSP;
- `minviolations`: uses the formulation by [Bautista et al. (2008)](https://link.springer.com/article/10.1007/s10479-007-0278-x), which can solve the Max-CSP if increasing $\alpha$ values are considered (combine with the `penalize` flag);
- `sos1`: selects formulation $F2$ implemented with a special ordered set of type 1 (SOS1), which can have increasing (combine with the `incsearch` flag – $F2_I$) or decreasing (combine with the `decsearch` flag – $F2_D$) weights, or weights mimicking a binary search (default – $F2_B$).

## Questions?

If you encounter any problems or have any questions, please feel free to send me an e-mail (laradicp@gmail.com).
