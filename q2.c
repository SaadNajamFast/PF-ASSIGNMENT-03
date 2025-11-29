#include <stdio.h>

int calculateFuel(int fuel, int consumption, int recharge, int solarBonus, int planet, int totalPlanets)
{
    if (fuel <= 0 || planet > totalPlanets)
        return fuel;

    fuel -= consumption;
    fuel += recharge;

    if (planet % 4 == 0)
        fuel += solarBonus;

    if (fuel < 0)
        fuel = 0;

    printf("Planet %d: Fuel Remaining = %d\n", planet, fuel);

    return calculateFuel(fuel, consumption, recharge, solarBonus, planet + 1, totalPlanets);
}

int main()
{
    int fuel = 500;
    int consumption = 60;
    int recharge = 20;
    int solarBonus = 50;
    int totalPlanets = 10;

    int remaining = calculateFuel(fuel, consumption, recharge, solarBonus, 1, totalPlanets);

    printf("\nFinal Fuel = %d\n", remaining);

    if (remaining > 0)
        printf("Journey Completed\n");
    else
        printf("Fuel Exhausted. Mission Failed\n");

    return 0;
}