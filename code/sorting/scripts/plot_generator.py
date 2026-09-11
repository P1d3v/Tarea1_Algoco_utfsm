import pandas as pd
import matplotlib.pyplot as plt
import os


def obtener_caracteristica(nombre_archivo):
    for tipo in ("aleatorio", "ascendente", "descendente"):
        if f"_{tipo}_" in nombre_archivo:
            return tipo
    return "no especificada"


def obtener_coincidencias_memoria(series_memoria, algoritmo_referencia):
    coincidencias = {}
    serie_referencia = series_memoria.get(algoritmo_referencia)

    if serie_referencia is None:
        return coincidencias

    for algoritmo, serie in series_memoria.items():
        if algoritmo == algoritmo_referencia:
            continue

        tamanos_comunes = serie_referencia.index.intersection(serie.index)
        tamanos_iguales = tamanos_comunes[
            serie_referencia.loc[tamanos_comunes].eq(serie.loc[tamanos_comunes])
        ]

        if not tamanos_iguales.empty:
            coincidencias[algoritmo] = list(tamanos_iguales)

    return coincidencias

ruta_csv = r'C:\Users\felip\Desktop\T1\code\sorting\data\measurements\a.csv'
ruta_plots = r'C:\Users\felip\Desktop\T1\code\sorting\data\plots'

os.makedirs(ruta_plots, exist_ok=True)

df = pd.read_csv(ruta_csv, sep=';')
df['Dominio'] = df['Archivo de entrada'].str.extract(r'_(D\d+)_')
df['Caracteristica del arreglo'] = df['Archivo de entrada'].map(obtener_caracteristica)

dominios = df['Dominio'].unique()
tipos = df['Caracteristica del arreglo'].unique()
algoritmos = df['Nombre del algoritmo'].unique()

for dominio in dominios:
    for tipo in tipos:
        datos_escenario = df[(df['Dominio'] == dominio) & (df['Caracteristica del arreglo'] == tipo)]
        
        if not datos_escenario.empty:
            plt.figure(figsize=(10, 6))
            
            for algoritmo in algoritmos:
                datos_alg = datos_escenario[datos_escenario['Nombre del algoritmo'] == algoritmo]
                
                if not datos_alg.empty:
                    promedios = datos_alg.groupby('Tamano del arreglo')['Tiempo'].mean().sort_index()
                    plt.plot(promedios.index, promedios.values, marker='o', linestyle='-', linewidth=2, label=algoritmo)
            
            plt.title(f'Tiempo vs Tamaño | Dominio: {dominio} | Tipo: {tipo}')
            plt.xlabel('Tamaño del arreglo (N)')
            plt.ylabel('Tiempo promedio (ms)')
            plt.xscale('log') 
            plt.legend(title="Algoritmos")
            plt.grid(True, linestyle='--', alpha=0.7)
            plt.tight_layout()
            
            nombre_imagen = f'lineas_{dominio}_{tipo}.png'
            ruta_completa = os.path.join(ruta_plots, nombre_imagen)
            
            plt.savefig(ruta_completa)
            plt.close()
for dominio in dominios:
    for tipo in tipos:
        datos_escenario = df[(df['Dominio'] == dominio) & 
                             (df['Caracteristica del arreglo'] == tipo)]
        
        if not datos_escenario.empty:
            plt.figure(figsize=(10, 6))
            series_memoria = {}

            for algoritmo in algoritmos:
                datos_alg = datos_escenario[datos_escenario['Nombre del algoritmo'] == algoritmo]
                if not datos_alg.empty:
                    promedios_memoria = datos_alg.groupby('Tamano del arreglo')['Memoria usada'].mean().sort_index()
                    series_memoria[algoritmo] = promedios_memoria
                    plt.plot(promedios_memoria.index, promedios_memoria.values, marker='s', linestyle='-', linewidth=2, label=algoritmo)

            coincidencias = obtener_coincidencias_memoria(series_memoria, 'merge_sort')
            if coincidencias:
                avisos = [
                    f"merge_sort coincide con {algoritmo} en N={', '.join(map(str, tamanos))}"
                    for algoritmo, tamanos in coincidencias.items()
                ]
                plt.gca().text(
                    0.02,
                    0.98,
                    'Aviso:\n' + '\n'.join(avisos),
                    transform=plt.gca().transAxes,
                    va='top',
                    bbox=dict(boxstyle='round,pad=0.5', facecolor='lightyellow', edgecolor='darkgoldenrod', alpha=0.9)
                )
            
            plt.title(f'Memoria vs Tamaño | Dominio: {dominio} | Tipo: {tipo}')
            plt.xlabel('Tamaño del arreglo (N)')
            plt.ylabel('Memoria promedio (bytes)')
            plt.xscale('log') 
            plt.legend(title="Algoritmos")
            plt.grid(True, linestyle='--', alpha=0.7)
            plt.tight_layout()
            
            nombre_imagen_memoria = f'memoria_lineas_{dominio}_{tipo}.png'
            plt.savefig(os.path.join(ruta_plots, nombre_imagen_memoria))
            plt.close()
print("Se hizo")
