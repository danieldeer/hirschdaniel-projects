#include <stdio.h>
#include <curl/curl.h>

size_t write_data(void *buffer, size_t size, size_t nmemb, void *userp)
{
    char c;
    for(int i=0; i<nmemb; i++)
    {
        c = ((char *) buffer)[i];
        printf("%c", c);
    }
    return nmemb; 
}

int main()
{
    printf("Welcome to the Ultimate C HTTP Application\n");
    
    curl_global_init(CURL_GLOBAL_ALL);
    
    CURL *handle = curl_easy_init();
    curl_easy_setopt(handle, CURLOPT_URL, "https://api.open-meteo.com/v1/forecast?latitude=52.52&longitude=13.41&current=temperature_2m,wind_speed_10m&hourly=temperature_2m,relative_humidity_2m,wind_speed_10m");
    curl_easy_setopt(handle, CURLOPT_WRITEFUNCTION, write_data);
    
    CURLcode response = curl_easy_perform(handle);
    if (response == 0)
    {
        printf("Success.\n");
    }
    else
    {
        printf("Failed request. Code: %d\n", response);
    }
}

