/* checks the stored values for each rock characteristic
determines the rock type based on these measurements 
B = Basaltoid
G = Gravion
R = Regolix
L = Lunarite */

char classify(int mag, int ir, int us) {
    if (mag  == 0 || ir == 0) {
        return '-';
    }

    if (ir == 547) {
        if (mag == -1 || us == 1) {
            return 'B';
        }
        else {
            return 'L';
        }
    }
    else {
        if (mag == -1 || us == 0) {
            return 'G';
        }
        else {
            return 'R';
        }
    }
}