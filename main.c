#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>

/*
 * Lab0 - Query Anthropic API about the GLM-5.3 security research
 * Report: "GLM-5.3 and the spread of advanced cyber capabilities" (2026-09-29)
 * https://www.anthropic.com/research/glm-5-3-and-the-spread-of-advanced-cyber-capabilities
 */

#define API_URL "https://api.anthropic.com/v1/messages"
#define API_VERSION "2023-06-01"

struct response_buffer {
    char *data;
    size_t size;
};

size_t write_callback(void *contents, size_t size, size_t nmemb, void *userp) {
    size_t total_size = size * nmemb;
    struct response_buffer *buffer = (struct response_buffer *)userp;

    char *ptr = realloc(buffer->data, buffer->size + total_size + 1);
    if (!ptr) {
        fprintf(stderr, "Memory allocation failed\n");
        return 0;
    }

    buffer->data = ptr;
    memcpy(&(buffer->data[buffer->size]), contents, total_size);
    buffer->size += total_size;
    buffer->data[buffer->size] = 0;

    return total_size;
}

void load_env(const char *filepath) {
    FILE *file = fopen(filepath, "r");
    if (!file) return;

    char line[512];
    while (fgets(line, sizeof(line), file)) {
        if (line[0] == '#' || line[0] == '\n') continue;

        char *eq = strchr(line, '=');
        if (!eq) continue;

        *eq = '\0';
        char *key = line;
        char *value = eq + 1;

        char *newline = strchr(value, '\n');
        if (newline) *newline = '\0';

        setenv(key, value, 0);
    }
    fclose(file);
}

int main(void) {
    load_env(".env");

    const char *api_key = getenv("ANTHROPIC_API_KEY");
    const char *base_url = getenv("ANTHROPIC_BASE_URL");

    if (!api_key) {
        fprintf(stderr, "Error: ANTHROPIC_API_KEY not found in .env file\n");
        fprintf(stderr, "Please create a .env file with your API key\n");
        return 1;
    }

    if (!base_url) {
        base_url = API_URL;
    }

    CURL *curl = curl_easy_init();
    if (!curl) {
        fprintf(stderr, "Failed to initialize CURL\n");
        return 1;
    }

    struct response_buffer response = {0};
    response.data = malloc(1);
    response.size = 0;

    const char *json_payload =
        "{"
        "\"model\": \"claude-opus-4-6\","
        "\"max_tokens\": 1024,"
        "\"messages\": ["
        "  {"
        "    \"role\": \"user\","
        "    \"content\": \"Summarize in 3 key points: what makes the GLM-5.3 model release concerning from a cybersecurity perspective?\""
        "  }"
        "]"
        "}";

    struct curl_slist *headers = NULL;
    char auth_header[512];
    snprintf(auth_header, sizeof(auth_header), "x-api-key: %s", api_key);

    headers = curl_slist_append(headers, "Content-Type: application/json");
    headers = curl_slist_append(headers, auth_header);
    headers = curl_slist_append(headers, "anthropic-version: " API_VERSION);

    curl_easy_setopt(curl, CURLOPT_URL, base_url);
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, json_payload);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&response);

    printf("Querying Claude API about GLM-5.3 security concerns...\n\n");

    CURLcode res = curl_easy_perform(curl);

    if (res != CURLE_OK) {
        fprintf(stderr, "Request failed: %s\n", curl_easy_strerror(res));
        curl_slist_free_all(headers);
        curl_easy_cleanup(curl);
        free(response.data);
        return 1;
    }

    printf("API Response:\n%s\n", response.data);

    curl_slist_free_all(headers);
    curl_easy_cleanup(curl);
    free(response.data);

    return 0;
}
