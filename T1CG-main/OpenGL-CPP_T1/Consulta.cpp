#include <chrono>
#include "Consulta.h"
#include "ListaDeCoresRGB.h"

ResultadoConsulta executaConsulta(Ponto p,
                                  std::vector<Triangulo>& triangulos,
                                  NoQuadtree& raiz,
                                  bool usaQuadtree,
                                  bool usaEnvelope)
{
    ResultadoConsulta r;
    r.totalTriangulos = triangulos.size();

    auto inicio = std::chrono::steady_clock::now();

    // subdivisao espacial 
    const std::vector<int>* candidatos = NULL;   // todos os triangulos
    int n = triangulos.size();

    if (usaQuadtree){
        r.regiao = localizaRegiaoDoPonto(p, raiz);
        if (r.regiao != NULL){
            candidatos = &r.regiao->indicesTriangulo;
            n = candidatos->size();
        }
        // Se o ponto estiver fora da raiz (ex.: arrastando o mouse para fora da
        // janela), a quadtree nao cobre essa area: triangulos que saem da janela
        // podem conter o ponto e nao estao em nenhuma folha. Nesse caso a
        // subdivisao nao descarta nada (candidatos = todos), mantendo o resultado
        // igual ao da forca bruta.
    }
    r.candidatosQuadtree = n;

    for (int k = 0; k < n; k++){
        int i = (candidatos != NULL) ? (*candidatos)[k] : k;

        // envelope 
        if (usaEnvelope){
            if (!triangulos[i].PontoNoAABB(p)){
                continue;
            }
            r.aprovados.push_back(i);
        }

        //teste 
        r.testesDetalhados++;
        if (triangulos[i].PontoNoTriangulo(p)){
            r.contem.push_back(i);
        }
    }

    // Sem envelope, todos os candidatos "passam" para o teste detalhado
    r.aprovadosEnvelope = usaEnvelope ? r.aprovados.size() : n;

    auto fim = std::chrono::steady_clock::now();
    r.tempoMicros = std::chrono::duration<double, std::micro>(fim - inicio).count();

    return r;
}

// **********************************************************************
// void desenhaCandidadtosDaQuadtree(ResultadoConsulta r, std::vector<Triangulo> triangulos)
//      Desenha os triangulos candidatos que passaram no teste da quadtree
//      Parametro é a consulta para pegar a quantidade de candidatos e os indices
//      e o total de triangulos presentes no programa original
// **********************************************************************

void desenhaCandidatosDaQuadtree(const ResultadoConsulta& r, std::vector<Triangulo>& Triangulos){

    if (r.regiao == NULL) return;   // quadtree desligada: nao ha regiao
    glLineWidth(2);
    glColor3f(1.0f, 0.55f, 0.0f);   // laranja
    for (int i = 0; i < (int)r.regiao->indicesTriangulo.size(); i++){
        Triangulos[r.regiao->indicesTriangulo[i]].DesenhaArestas();
    }
    glLineWidth(1);
}

// **********************************************************************
// void desenhaAprovadosPeloEnvelope(ResultadoConsulta r, std::vector<Triangulo> triangulos)
//      Desenha os triangulos candidatos foram aprovados no teste de envelope
//      Parametro é a consulta para pegar os triangulos aprovados e os indices
//      e o total de triangulos presentes no programa original
// **********************************************************************

void desenhaAprovadosPeloEnvelope(const ResultadoConsulta& r, std::vector<Triangulo>& Triangulos){

    // (o "Violet" da tabela de cores e um roxo muito escuro, quase preto)
    glLineWidth(3);
    glColor3f(0.85f, 0.0f, 0.85f);  // magenta
    for (int i = 0; i < (int)r.aprovados.size(); i++){
        Triangulos[r.aprovados[i]].DesenhaArestas();
    }
    glLineWidth(1);
}