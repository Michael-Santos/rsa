# Dependências


É preciso instalar a biblioteca [GMP](https://gmplib.org/manual/Installing-GMP.html).


# RSA

O programa é capaz de gerar chaves e de encriptar/decriptar mensagens.

# Problemas

Tive um problema ao instalar a GMP:

`
checking for suitable m4... configure: error: No usable m4 in $PATH or /usr/5bin (see config.log for reasons)
`

Para resolver for preciso instalar a dependência m4: `# apt install m4`