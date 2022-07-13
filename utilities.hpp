double closest(const double numbers[], int size, double target) {
    double closest = numbers[0];
    for (int i = 0; i < size; i++) {
        if (abs(target - numbers[i]) < abs(target - closest)) {
            closest = numbers[i];
        }
    }
    return closest;
}