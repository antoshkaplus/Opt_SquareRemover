#pragma once


const static vector<Count> kMinSqMoves = {
    3, 3, 3, 3, 3, 3, 3, 3, 3
};


const static vector<double> kLocFactors = {
    0.3, 0.25, 0.25, 0.55, 0.75, 0.8, 1, 0.9, 0.98
};

const static vector<double> kTripleFactors = {
    0, 0, 0, 0, 0, 0, 1, 1, 8
};

const static vector<double> kDoubleFactors = {
    0.35972, 0.471176, 0.348484, 0.382926, 0.494382, 0.2796, 0.382926, 0.627124, 0.841906
};
// for bigger triple population
//const static vector<double> kTripleFactors = {
//0.622099, 0.235108, 0.00406531, 0.300886, 0.608944, 0.995935, 0.218847, 0.412343, 0.648411
//};

// only if you count sq and triples.
const static vector<Count> kBeamWidth = {
    370,
    200,
    80,
    900/5,
    550/3,
    450/7,
    450*3,
    700,
    700
};