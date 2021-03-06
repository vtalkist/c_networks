#include <sys/socket.h>
#include <netdb.h>
#include <ifaddrs.h>
#include <stdio.h>
#include <stdlib.h>


int main() {

    /*******************************************
    * Оголошення структури для зберігання адрес
    ********************************************/
   
    struct ifaddrs *addresses;

    /****************************************************************************** 
    * getifaddr() виділяє пам'ять і наповнює зв'язаний список адресами або повертає
    * -1 у випадку невдачі
    *******************************************************************************/

    if (getifaddrs(&addresses) == -1) {
        printf("getifaddrs call failed\n");
        return -1;
    }

    /******************************************************************************
     * Використання вказівника *address для проходження по зв'язаному списку адрес.
     * Після розглядання адреси, перекидується вказівник на наступну, доки не 
     * кінець списку
     ******************************************************************************/

    struct ifaddrs *address = addresses;
    while(address) {
        if (address->ifa_addr == NULL) { 
            address = address->ifa_next;
            continue;
        }

        /******************************************************************
         * Для кожної адреси індетифікується сімейство адреси (IPv4/IPv6)
        *******************************************************************/

        int family = address->ifa_addr->sa_family;
        /****************************************************************
         * Оголошення та ініціалізація змінної family значення sa_family
         * для подальшого друку назви сімейства в control flow
        ****************************************************************/
        if (family == AF_INET || family == AF_INET6) {

            printf("%s\t", address->ifa_name);
            printf("%s\t", family == AF_INET ? "IPv4" : "IPv6");

            /******************************************************
             * Створення буферу ар для збереження текстової адреси
             * заповнення його значенням, вивід на консоль
            ******************************************************/
            char ap[100];
            const int family_size = family == AF_INET ?
                sizeof(struct sockaddr_in) : sizeof(struct sockaddr_in6);
            getnameinfo(address->ifa_addr,
                    family_size, ap, sizeof(ap), 0, 0, NI_NUMERICHOST);
            printf("\t%s\n", ap);

        }
        address = address->ifa_next;
    }
    /*******************************************************
     * Звільнення ДП, що була виділена функцією getifaddrs()
    ********************************************************/
   
    freeifaddrs(addresses);
    return 0;
}