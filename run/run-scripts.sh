#!/bin/bash

./run-heuristic.sh && ./run-lit-asc-it-heur.sh & ./run-lit-bin-heur-comb.sh & ./run-lit-bin-heur-triv.sh & ./run-lit-desc-it-comb-heur-primal.sh & ./run-lit-regular-heur-primal.sh & ./run-lit-sos1-heur-primal.sh & ./run-real.sh &
