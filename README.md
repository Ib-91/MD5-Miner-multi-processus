# MD5-Miner-multi-processus
Le programme simule un minage simplifié inspiré de Bitcoin. Il recherche par force brute (bruteforce) un nonce dont le hachage MD5 commence par un nombre donné de zéros. Le travail est réparti entre plusieurs processus créés avec fork. Dès qu’un processus trouve une solution valide, le processus parent interrompt immédiatement tous les autres.
