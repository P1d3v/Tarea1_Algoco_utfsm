import pandas as pd
import matplotlib.pyplot as plt
import os

ruta_csv = r'C:\Users\felip\Desktop\T1\code\matrix_multiplication\data\measurements\a.csv'
ruta_plots = r'C:\Users\felip\Desktop\T1\code\matrix_multiplication\data\plots'

os.makedirs(ruta_plots, exist_ok=True)

df = pd.read_csv(ruta_csv, sep=';')

df['Tamano de la matriz'] = (
    df['Tamano de la matriz']
    .str.extract(r'^(\d+)', expand=False)
    .astype(int)
)

df['Dominio'] = df['Archivo de entrada'].str.extract(r'_(D\d+)_')

dominios = df['Dominio'].unique()
tipos = df['Caracteristica de la matriz'].unique()
algoritmos = df['Nombre del algoritmo'].unique()

for dominio in dominios:
    for tipo in tipos:
        datos_escenario = df[(df['Dominio'] == dominio) & 
                             (df['Caracteristica de la matriz'] == tipo)]
        
        if not datos_escenario.empty:
            plt.figure(figsize=(10, 6))
            for algoritmo in algoritmos:
                datos_alg = datos_escenario[datos_escenario['Nombre del algoritmo'] == algoritmo]
                if not datos_alg.empty:
                    promedios_tiempo = datos_alg.groupby('Tamano de la matriz')['Tiempo'].mean().sort_index()
                    plt.plot(promedios_tiempo.index, promedios_tiempo.values, marker='o', linestyle='-', linewidth=2, label=algoritmo)
            
            plt.title(f'Tiempo vs Tamaño | Dominio: {dominio} | Tipo: {tipo}')
            plt.xlabel('Dimensión de la Matriz (N)')
            plt.ylabel('Tiempo promedio (ms)')
            plt.xscale('log', base=2) 
            plt.legend(title="Algoritmos")
            plt.grid(True, linestyle='--', alpha=0.7)
            plt.tight_layout()
            
            nombre_imagen_tiempo = f'tiempo_matrices_{dominio}_{tipo}.png'
            plt.savefig(os.path.join(ruta_plots, nombre_imagen_tiempo))
            plt.close()

for dominio in dominios:
    for tipo in tipos:
        datos_escenario = df[(df['Dominio'] == dominio) & 
                             (df['Caracteristica de la matriz'] == tipo)]
        
        if not datos_escenario.empty:
            plt.figure(figsize=(10, 6))
            for algoritmo in algoritmos:
                datos_alg = datos_escenario[datos_escenario['Nombre del algoritmo'] == algoritmo]
                if not datos_alg.empty:
                    promedios_memoria = datos_alg.groupby('Tamano de la matriz')['Memoria usada'].mean().sort_index()
                    plt.plot(promedios_memoria.index, promedios_memoria.values, marker='s', linestyle='-', linewidth=2, label=algoritmo)
            
            plt.title(f'Memoria vs Tamaño | Dominio: {dominio} | Tipo: {tipo}')
            plt.xlabel('Dimensión de la Matriz (N)')
            plt.ylabel('Memoria promedio (bytes)')
            plt.xscale('log', base=2) 
            plt.legend(title="Algoritmos")
            plt.grid(True, linestyle='--', alpha=0.7)
            plt.tight_layout()
            
            nombre_imagen_memoria = f'memoria_matrices_{dominio}_{tipo}.png'
            plt.savefig(os.path.join(ruta_plots, nombre_imagen_memoria))
            plt.close()

print(f"Graficos de matrices guardados en: {ruta_plots}")