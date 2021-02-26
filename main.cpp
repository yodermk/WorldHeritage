#include "Controller.h"

int main() {
    Controller c;
    if (c.load_file("whc-sites.csv"))
        return 1;
    c.run();
}
