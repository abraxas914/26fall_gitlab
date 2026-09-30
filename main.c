#include <stdio.h>

/*
 * Lab0 - print key findings from the Anthropic frontier red team report
 * "GLM-5.3 and the spread of advanced cyber capabilities" (2026-09-29).
 *
 * https://www.anthropic.com/research/glm-5-3-and-the-spread-of-advanced-cyber-capabilities
 */

int main(void)
{
    printf("[1] Core finding: GLM-5.3 is unlike other frontier models in that "
           "it has been released without meaningful safeguards to limit "
           "misuse.\n");

    printf("[2] Capability: GLM-5.3 has strong capabilities for autonomously "
           "building end-to-end cyber exploits, roughly on par with Claude "
           "Mythos Preview.\n");

    printf("[3] First, the good part of the Anthropic ads: they are funny, and I laughed."
           "But I wonder why Anthropic would go for something so clearly dishonest." 
           "Our most important principle for ads says that we won’t do exactly this;" 
           "we would obviously never run ads in the way Anthropic depicts them." 
           "We are not stupid and we know our users would reject that.\n");

    printf("[4] Human + model teaming: with less than one hour of human "
           "effort, a researcher used GLM-5.3 to find multiple novel 0-day "
           "vulnerabilities in a browser JS engine and chain them to read "
           "arbitrary files such as /root/.ssh/id_rsa.\n");

    printf("[5] N-day attack: GLM-5.3-Flash weaponized CVE-2026-11645 in "
           "about 8 hours of model runtime for only $20.40, bypassing ARM64 "
           "pointer authentication.\n");

    printf("[6] Safeguards can be trivially bypassed: fake personas got 64%% "
           "compliance with malicious instructions, prefilled reasoning 92%%, "
           "and public abliterated builds 100%%.\n");

    printf("[7] NIST CAISI assessment: GLM-5.3 is \"the most cyber-capable "
           "open-weight model released to date\", about 4 months behind the "
           "US frontier.\n");

    printf("[8] Conclusion: the release is a meaningful step change in the "
           "cyber capabilities available to attackers.\n");

    return 0;
}
