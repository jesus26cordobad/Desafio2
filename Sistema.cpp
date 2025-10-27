#include "Sistema.h"
#include "Creditos.h"
#include "MedidorRecursos.h"
#include "MensajePublicitario.h"
#include <iostream>
#include <thread>
#include <chrono>

#ifdef _WIN32
#include <direct.h>
#define mkdir _mkdir
#else
#include <sys/stat.h>
#include <sys/types.h>
#endif

using namespace std;

// ---------------------------------------------------------------------
// Utilidad visual de demo (lista de usuarios creados)
// ---------------------------------------------------------------------
static void mostrarUsuariosDemo() {
    cout << "\n========================================" << endl;
    cout << "     USUARIOS DISPONIBLES EN UDEATUNES" << endl;
    cout << "========================================" << endl;
    cout << "PREMIUM:" << endl;
    cout << "  - juan123" << endl;
    cout << "  - pedro789" << endl;
    cout << "  - laura_music" << endl;
    cout << "  - sofia2024" << endl;
    cout << "  - camila_26" << endl;
    cout << "  - valentina_pro" << endl;
    cout << "\nESTANDAR:" << endl;
    cout << "  - maria456" << endl;
    cout << "  - carlos_a12" << endl;
    cout << "  - andres225" << endl;
    cout << "  - david1" << endl;
    cout << "========================================" << endl;
}
//Constructor: inicializa todos los gestores
Sistema::Sistema() {
    gestorUsuarios   = new GestorUsuarios();
    gestorArtistas   = new GestorArtistas();
    gestorPublicidad = new GestorPublicidad();
    reproductor      = 0;
    usuarioActual    = 0;
    sistemaActivo    = true;
}
//Destructor: libera memoria
Sistema::~Sistema() {
    delete gestorUsuarios;
    delete gestorArtistas;
    delete gestorPublicidad;
    delete reproductor;
}

// ---------------------------------------------------------------------
// Carga de datos o generación de demo
// ---------------------------------------------------------------------
#include <sys/stat.h>
#include <sys/types.h>

void Sistema::cargarDatos() {
//Crear carpeta data si no existe
#ifdef _WIN32
    _mkdir("data");
#else
    mkdir("data", 0777);
#endif

    //Intentar cargar datos desde archivos
    bool artistasOk = gestorArtistas->cargarDesdeArchivo("data/artistas.dat");
    bool publicidadOk = gestorPublicidad->cargarDesdeArchivo("data/publicidad.dat");
    bool usuariosOk = gestorUsuarios->cargarDesdeArchivo("data/usuarios.dat");

    if (!artistasOk || !publicidadOk || !usuariosOk) {
        cout << "Generando datos iniciales..." << endl;
        generarDatosPrueba();

        //Guardar inmediatamente después de generar
        cout << "Guardando datos..." << endl;
        gestorArtistas->guardarEnArchivo("data/artistas.dat");
        gestorPublicidad->guardarEnArchivo("data/publicidad.dat");
        gestorUsuarios->guardarEnArchivo("data/usuarios.dat");
        cout << "Datos guardados correctamente." << endl;
    } else {
        //Reconstruye favoritos desde IDs
        gestorUsuarios->reconstruirFavoritos(gestorArtistas);
    }
}

// ---------------------------------------------------------------------
// Demo: crea usuarios, catálogo, publicidad y favoritos de ejemplo
// ---------------------------------------------------------------------

void Sistema::generarDatosPrueba() {

    // ---------------- USUARIOS ----------------
    Usuario* u1 = new Usuario("juan123", "premium", "Medellin", "Colombia");
    u1->setFechaInscripcion("2024-01-15");

    Usuario* u2 = new Usuario("maria456", "estandar", "Bogota", "Colombia");
    u2->setFechaInscripcion("2024-02-20");

    Usuario* u3 = new Usuario("pedro789", "premium", "Cali", "Colombia");
    u3->setFechaInscripcion("2024-03-10");

    Usuario* u4 = new Usuario("laura_music", "premium", "Barranquilla", "Colombia");
    u4->setFechaInscripcion("2024-04-05");

    Usuario* u5 = new Usuario("carlos_a12", "estandar", "Cartagena", "Colombia");
    u5->setFechaInscripcion("2024-05-12");

    Usuario* u6 = new Usuario("sofia2024", "premium", "Bucaramanga", "Colombia");
    u6->setFechaInscripcion("2024-06-18");

    Usuario* u7 = new Usuario("andres225", "estandar", "Pereira", "Colombia");
    u7->setFechaInscripcion("2024-07-22");

    Usuario* u8 = new Usuario("camila_26", "premium", "Manizales", "Colombia");
    u8->setFechaInscripcion("2024-08-30");

    Usuario* u9 = new Usuario("david1", "estandar", "Santa Marta", "Colombia");
    u9->setFechaInscripcion("2024-09-14");

    Usuario* u10 = new Usuario("valentina_pro", "premium", "Armenia", "Colombia");
    u10->setFechaInscripcion("2024-10-01");

    gestorUsuarios->agregarUsuario(u1);
    gestorUsuarios->agregarUsuario(u2);
    gestorUsuarios->agregarUsuario(u3);
    gestorUsuarios->agregarUsuario(u4);
    gestorUsuarios->agregarUsuario(u5);
    gestorUsuarios->agregarUsuario(u6);
    gestorUsuarios->agregarUsuario(u7);
    gestorUsuarios->agregarUsuario(u8);
    gestorUsuarios->agregarUsuario(u9);
    gestorUsuarios->agregarUsuario(u10);

    // ---------------- CATÁLOGO MUSICAL MASIVO ----------------
    int totalCanciones = 0;

    // ========== ARTISTA 1: Shakira (ID: 12345) ==========
    Artista* a1 = new Artista(12345, 47, "Colombia");
    a1->setNombre("Shakira");
    a1->setCantidadSeguidores(5000000);
    a1->setPosicionTendencias(1);

    Album* al1 = new Album(1, "Las Mujeres Ya No Lloran", "2024-03-22", "Sony Music");
    al1->setRutaPortada("/users/storage/shakira/image/las_mujeres.png");
    al1->agregarGenero("Pop");
    al1->agregarGenero("Latina");

    // IDs: 12345 (artista) + 01 (álbum) + 01-10 (canciones)
    al1->agregarCancion(new Cancion(123450101L, "Me Enamore", 214, "/users/storage/shakira/audio/me_enamore"));
    al1->agregarCancion(new Cancion(123450102L, "Monotonia", 225, "/users/storage/shakira/audio/monotonia"));
    al1->agregarCancion(new Cancion(123450103L, "Perro Fiel", 195, "/users/storage/shakira/audio/perro_fiel"));
    al1->agregarCancion(new Cancion(123450104L, "Chantaje", 192, "/users/storage/shakira/audio/chantaje"));
    al1->agregarCancion(new Cancion(123450105L, "La Bicicleta", 209, "/users/storage/shakira/audio/la_bicicleta"));
    al1->agregarCancion(new Cancion(123450106L, "Te Felicito", 217, "/users/storage/shakira/audio/te_felicito"));
    al1->agregarCancion(new Cancion(123450107L, "Hips Dont Lie", 218, "/users/storage/shakira/audio/hips_dont_lie"));
    al1->agregarCancion(new Cancion(123450108L, "Waka Waka", 205, "/users/storage/shakira/audio/waka_waka"));
    al1->agregarCancion(new Cancion(123450109L, "Loca", 199, "/users/storage/shakira/audio/loca"));
    al1->agregarCancion(new Cancion(123450110L, "Whenever Wherever", 196, "/users/storage/shakira/audio/whenever"));
    totalCanciones += 10;

    a1->agregarAlbum(al1);
    gestorArtistas->agregarArtista(a1);

    // ========== ARTISTA 2: Karol G (ID: 67890) ==========
    Artista* a2 = new Artista(67890, 33, "Colombia");
    a2->setNombre("Karol G");
    a2->setCantidadSeguidores(8000000);
    a2->setPosicionTendencias(2);

    Album* al2 = new Album(1, "Manana Sera Bonito", "2023-02-24", "Universal");
    al2->setRutaPortada("/users/storage/karolg/image/manana_sera_bonito.png");
    al2->agregarGenero("Pop");
    al2->agregarGenero("Urbano");

    // IDs: 67890 (artista) + 01 (álbum) + 01-12 (canciones)
    al2->agregarCancion(new Cancion(678900101L, "Besties", 195, "/users/storage/karolg/audio/besties"));
    al2->agregarCancion(new Cancion(678900102L, "Sejodioto", 205, "/users/storage/karolg/audio/sejodioto"));
    al2->agregarCancion(new Cancion(678900103L, "Cairo", 187, "/users/storage/karolg/audio/cairo"));
    al2->agregarCancion(new Cancion(678900104L, "Provenza", 212, "/users/storage/karolg/audio/provenza"));
    al2->agregarCancion(new Cancion(678900105L, "Gatubela", 189, "/users/storage/karolg/audio/gatubela"));
    al2->agregarCancion(new Cancion(678900106L, "X Si Volvemos", 198, "/users/storage/karolg/audio/x_si_volvemos"));
    al2->agregarCancion(new Cancion(678900107L, "Amargura", 203, "/users/storage/karolg/audio/amargura"));
    al2->agregarCancion(new Cancion(678900108L, "Tus Gafitas", 191, "/users/storage/karolg/audio/tus_gafitas"));
    al2->agregarCancion(new Cancion(678900109L, "Qlona", 207, "/users/storage/karolg/audio/qlona"));
    al2->agregarCancion(new Cancion(678900110L, "Carolina", 201, "/users/storage/karolg/audio/carolina"));
    al2->agregarCancion(new Cancion(678900111L, "Tusa", 200, "/users/storage/karolg/audio/tusa"));
    al2->agregarCancion(new Cancion(678900112L, "Bichota", 193, "/users/storage/karolg/audio/bichota"));
    totalCanciones += 12;

    a2->agregarAlbum(al2);
    gestorArtistas->agregarArtista(a2);

    // ========== ARTISTA 3: Juanes (ID: 54321) ==========
    Artista* a3 = new Artista(54321, 51, "Colombia");
    a3->setNombre("Juanes");
    a3->setCantidadSeguidores(3000000);
    a3->setPosicionTendencias(3);

    Album* al3 = new Album(1, "Mis Planes Son Amarte", "2017-05-12", "Universal");
    al3->setRutaPortada("/users/storage/juanes/image/mis_planes_son_amarte.png");
    al3->agregarGenero("Rock");
    al3->agregarGenero("Pop");

    // IDs: 54321 (artista) + 01 (álbum) + 01-11 (canciones)
    al3->agregarCancion(new Cancion(543210101L, "A Dios Le Pido", 230, "/users/storage/juanes/audio/a_dios_le_pido"));
    al3->agregarCancion(new Cancion(543210102L, "Es Tarde", 203, "/users/storage/juanes/audio/es_tarde"));
    al3->agregarCancion(new Cancion(543210103L, "Hermosa Ingrata", 198, "/users/storage/juanes/audio/hermosa_ingrata"));
    al3->agregarCancion(new Cancion(543210104L, "Goodbye For Now", 220, "/users/storage/juanes/audio/goodbye"));
    al3->agregarCancion(new Cancion(543210105L, "La Camisa Negra", 213, "/users/storage/juanes/audio/la_camisa_negra"));
    al3->agregarCancion(new Cancion(543210106L, "Fotografia", 225, "/users/storage/juanes/audio/fotografia"));
    al3->agregarCancion(new Cancion(543210107L, "Me Enamora", 208, "/users/storage/juanes/audio/me_enamora"));
    al3->agregarCancion(new Cancion(543210108L, "Volverte a Ver", 221, "/users/storage/juanes/audio/volverte_a_ver"));
    al3->agregarCancion(new Cancion(543210109L, "Para Tu Amor", 195, "/users/storage/juanes/audio/para_tu_amor"));
    al3->agregarCancion(new Cancion(543210110L, "Nada Valgo Sin Tu Amor", 219, "/users/storage/juanes/audio/nada_valgo"));
    al3->agregarCancion(new Cancion(543210111L, "Yerbabuena", 214, "/users/storage/juanes/audio/yerbabuena"));
    totalCanciones += 11;

    a3->agregarAlbum(al3);
    gestorArtistas->agregarArtista(a3);

    // ========== ARTISTA 4: Maluma (ID: 11111) ==========
    Artista* a4 = new Artista(11111, 30, "Colombia");
    a4->setNombre("Maluma");
    a4->setCantidadSeguidores(7500000);
    a4->setPosicionTendencias(4);

    Album* al4 = new Album(1, "Papi Juancho", "2020-08-21", "Sony Latin");
    al4->setRutaPortada("/users/storage/maluma/image/papi_juancho.png");
    al4->agregarGenero("Urbano");
    al4->agregarGenero("Reggae");

    // IDs: 11111 (artista) + 01 (álbum) + 01-12 (canciones)
    al4->agregarCancion(new Cancion(111110101L, "Hawai", 207, "/users/storage/maluma/audio/hawai"));
    al4->agregarCancion(new Cancion(111110102L, "Felices los 4", 230, "/users/storage/maluma/audio/felices_los_4"));
    al4->agregarCancion(new Cancion(111110103L, "Corazon", 189, "/users/storage/maluma/audio/corazon"));
    al4->agregarCancion(new Cancion(111110104L, "El Perdedor", 218, "/users/storage/maluma/audio/el_perdedor"));
    al4->agregarCancion(new Cancion(111110105L, "Borro Cassette", 195, "/users/storage/maluma/audio/borro_cassette"));
    al4->agregarCancion(new Cancion(111110106L, "Sobrio", 201, "/users/storage/maluma/audio/sobrio"));
    al4->agregarCancion(new Cancion(111110107L, "Admv", 213, "/users/storage/maluma/audio/admv"));
    al4->agregarCancion(new Cancion(111110108L, "Marry Me", 198, "/users/storage/maluma/audio/marry_me"));
    al4->agregarCancion(new Cancion(111110109L, "Coco Loco", 187, "/users/storage/maluma/audio/coco_loco"));
    al4->agregarCancion(new Cancion(111110110L, "Mama Tetema", 205, "/users/storage/maluma/audio/mama_tetema"));
    al4->agregarCancion(new Cancion(111110111L, "Medallo City", 197, "/users/storage/maluma/audio/medallo_city"));
    al4->agregarCancion(new Cancion(111110112L, "Parce", 203, "/users/storage/maluma/audio/parce"));
    totalCanciones += 12;

    a4->agregarAlbum(al4);
    gestorArtistas->agregarArtista(a4);

    // ========== ARTISTA 5: Carlos Vives (ID: 22222) ==========
    Artista* a5 = new Artista(22222, 62, "Colombia");
    a5->setNombre("Carlos Vives");
    a5->setCantidadSeguidores(4200000);
    a5->setPosicionTendencias(5);

    Album* al5 = new Album(1, "Cumbiana", "2020-06-19", "Sony Music");
    al5->setRutaPortada("/users/storage/carlosvives/image/cumbiana.png");
    al5->agregarGenero("Latina");
    al5->agregarGenero("Pop");

    // IDs: 22222 (artista) + 01 (álbum) + 01-10 (canciones)
    al5->agregarCancion(new Cancion(222220101L, "La Gota Fria", 215, "/users/storage/carlosvives/audio/la_gota_fria"));
    al5->agregarCancion(new Cancion(222220102L, "Volvi a Nacer", 227, "/users/storage/carlosvives/audio/volvi_a_nacer"));
    al5->agregarCancion(new Cancion(222220103L, "Fruta Fresca", 192, "/users/storage/carlosvives/audio/fruta_fresca"));
    al5->agregarCancion(new Cancion(222220104L, "Carito", 203, "/users/storage/carlosvives/audio/carito"));
    al5->agregarCancion(new Cancion(222220105L, "No Te Vayas", 198, "/users/storage/carlosvives/audio/no_te_vayas"));
    al5->agregarCancion(new Cancion(222220106L, "Cuando Nos Volvamos a Encontrar", 221, "/users/storage/carlosvives/audio/cuando_nos_volvamos"));
    al5->agregarCancion(new Cancion(222220107L, "Manana", 188, "/users/storage/carlosvives/audio/manana"));
    al5->agregarCancion(new Cancion(222220108L, "Como Le Gusta a Tu Cuerpo", 209, "/users/storage/carlosvives/audio/como_le_gusta"));
    al5->agregarCancion(new Cancion(222220109L, "Dile", 195, "/users/storage/carlosvives/audio/dile"));
    al5->agregarCancion(new Cancion(222220110L, "Robarte un Beso", 217, "/users/storage/carlosvives/audio/robarte_un_beso"));
    totalCanciones += 10;

    a5->agregarAlbum(al5);
    gestorArtistas->agregarArtista(a5);

    // ========== ARTISTA 6: J Balvin (ID: 33333) ==========
    Artista* a6 = new Artista(33333, 39, "Colombia");
    a6->setNombre("J Balvin");
    a6->setCantidadSeguidores(9000000);
    a6->setPosicionTendencias(6);

    Album* al6 = new Album(1, "Colores", "2020-03-19", "Universal Latin");
    al6->setRutaPortada("/users/storage/jbalvin/image/colores.png");
    al6->agregarGenero("Urbano");
    al6->agregarGenero("Reggae");

    // IDs: 33333 (artista) + 01 (álbum) + 01-11 (canciones)
    al6->agregarCancion(new Cancion(333330101L, "Rojo", 184, "/users/storage/jbalvin/audio/rojo"));
    al6->agregarCancion(new Cancion(333330102L, "Azul", 197, "/users/storage/jbalvin/audio/azul"));
    al6->agregarCancion(new Cancion(333330103L, "Blanco", 201, "/users/storage/jbalvin/audio/blanco"));
    al6->agregarCancion(new Cancion(333330104L, "Morado", 189, "/users/storage/jbalvin/audio/morado"));
    al6->agregarCancion(new Cancion(333330105L, "Verde", 193, "/users/storage/jbalvin/audio/verde"));
    al6->agregarCancion(new Cancion(333330106L, "Rosa", 206, "/users/storage/jbalvin/audio/rosa"));
    al6->agregarCancion(new Cancion(333330107L, "Amarillo", 211, "/users/storage/jbalvin/audio/amarillo"));
    al6->agregarCancion(new Cancion(333330108L, "Negro", 198, "/users/storage/jbalvin/audio/negro"));
    al6->agregarCancion(new Cancion(333330109L, "Gris", 187, "/users/storage/jbalvin/audio/gris"));
    al6->agregarCancion(new Cancion(333330110L, "Arcoiris", 215, "/users/storage/jbalvin/audio/arcoiris"));
    al6->agregarCancion(new Cancion(333330111L, "Ritmo", 209, "/users/storage/jbalvin/audio/ritmo"));
    totalCanciones += 11;

    a6->agregarAlbum(al6);
    gestorArtistas->agregarArtista(a6);

    // ========== ARTISTA 7: Feid (ID: 44444) ==========
    Artista* a7 = new Artista(44444, 31, "Colombia");
    a7->setNombre("Feid");
    a7->setCantidadSeguidores(6500000);
    a7->setPosicionTendencias(7);

    Album* al7 = new Album(1, "Feliz Cumpleanos Ferxxo", "2022-01-20", "Universal");
    al7->setRutaPortada("/users/storage/feid/image/ferxxo.png");
    al7->agregarGenero("Urbano");
    al7->agregarGenero("Reggae");

    // IDs: 44444 (artista) + 01 (álbum) + 01-10 (canciones)
    al7->agregarCancion(new Cancion(444440101L, "Normal", 188, "/users/storage/feid/audio/normal"));
    al7->agregarCancion(new Cancion(444440102L, "Si Tu Supieras", 195, "/users/storage/feid/audio/si_tu_supieras"));
    al7->agregarCancion(new Cancion(444440103L, "Porfa", 203, "/users/storage/feid/audio/porfa"));
    al7->agregarCancion(new Cancion(444440104L, "Hey Mor", 197, "/users/storage/feid/audio/hey_mor"));
    al7->agregarCancion(new Cancion(444440105L, "Fumeteo", 201, "/users/storage/feid/audio/fumeteo"));
    al7->agregarCancion(new Cancion(444440106L, "Bubalu", 189, "/users/storage/feid/audio/bubalu"));
    al7->agregarCancion(new Cancion(444440107L, "Feliz Cumpleanos", 207, "/users/storage/feid/audio/feliz_cumpleanos"));
    al7->agregarCancion(new Cancion(444440108L, "Fresh Kerias", 193, "/users/storage/feid/audio/fresh_kerias"));
    al7->agregarCancion(new Cancion(444440109L, "Chimbita", 185, "/users/storage/feid/audio/chimbita"));
    al7->agregarCancion(new Cancion(444440110L, "X20X", 199, "/users/storage/feid/audio/x20x"));
    totalCanciones += 10;

    a7->agregarAlbum(al7);
    gestorArtistas->agregarArtista(a7);

    // ========== ARTISTA 8: Sebastian Yatra (ID: 55555) ==========
    Artista* a8 = new Artista(55555, 29, "Colombia");
    a8->setNombre("Sebastian Yatra");
    a8->setCantidadSeguidores(5800000);
    a8->setPosicionTendencias(8);

    Album* al8 = new Album(1, "Dharma", "2022-01-28", "Universal");
    al8->setRutaPortada("/users/storage/yatra/image/dharma.png");
    al8->agregarGenero("Pop");
    al8->agregarGenero("Latina");

    // IDs: 55555 (artista) + 01 (álbum) + 01-12 (canciones)
    al8->agregarCancion(new Cancion(555550101L, "Tacones Rojos", 206, "/users/storage/yatra/audio/tacones_rojos"));
    al8->agregarCancion(new Cancion(555550102L, "Traicionera", 195, "/users/storage/yatra/audio/traicionera"));
    al8->agregarCancion(new Cancion(555550103L, "Robarte un Beso", 217, "/users/storage/yatra/audio/robarte_un_beso"));
    al8->agregarCancion(new Cancion(555550104L, "Cristina", 203, "/users/storage/yatra/audio/cristina"));
    al8->agregarCancion(new Cancion(555550105L, "Pareja del Ano", 199, "/users/storage/yatra/audio/pareja_del_ano"));
    al8->agregarCancion(new Cancion(555550106L, "Un Ano", 221, "/users/storage/yatra/audio/un_ano"));
    al8->agregarCancion(new Cancion(555550107L, "Tarde", 188, "/users/storage/yatra/audio/tarde"));
    al8->agregarCancion(new Cancion(555550108L, "Melancolia", 213, "/users/storage/yatra/audio/melancolia"));
    al8->agregarCancion(new Cancion(555550109L, "Como Mirarte", 197, "/users/storage/yatra/audio/como_mirarte"));
    al8->agregarCancion(new Cancion(555550110L, "Red Heels", 205, "/users/storage/yatra/audio/red_heels"));
    al8->agregarCancion(new Cancion(555550111L, "Vagabundo", 211, "/users/storage/yatra/audio/vagabundo"));
    al8->agregarCancion(new Cancion(555550112L, "Dharma", 193, "/users/storage/yatra/audio/dharma"));
    totalCanciones += 12;

    a8->agregarAlbum(al8);
    gestorArtistas->agregarArtista(a8);

    // ========== ARTISTA 9: Morat (ID: 66666) ==========
    Artista* a9 = new Artista(66666, 8, "Colombia");
    a9->setNombre("Morat");
    a9->setCantidadSeguidores(4500000);
    a9->setPosicionTendencias(9);

    Album* al9 = new Album(1, "Si Ayer Fuera Hoy", "2022-09-02", "Universal");
    al9->setRutaPortada("/users/storage/morat/image/si_ayer_fuera_hoy.png");
    al9->agregarGenero("Pop");
    al9->agregarGenero("Rock");

    // IDs: 66666 (artista) + 01 (álbum) + 01-11 (canciones)
    al9->agregarCancion(new Cancion(666660101L, "A Donde Vamos", 209, "/users/storage/morat/audio/a_donde_vamos"));
    al9->agregarCancion(new Cancion(666660102L, "Amor Con Hielo", 197, "/users/storage/morat/audio/amor_con_hielo"));
    al9->agregarCancion(new Cancion(666660103L, "Cuando Nadie Ve", 215, "/users/storage/morat/audio/cuando_nadie_ve"));
    al9->agregarCancion(new Cancion(666660104L, "No Termino", 203, "/users/storage/morat/audio/no_termino"));
    al9->agregarCancion(new Cancion(666660105L, "Presiento", 189, "/users/storage/morat/audio/presiento"));
    al9->agregarCancion(new Cancion(666660106L, "506", 221, "/users/storage/morat/audio/506"));
    al9->agregarCancion(new Cancion(666660107L, "Enamorate de Alguien Mas", 207, "/users/storage/morat/audio/enamorate"));
    al9->agregarCancion(new Cancion(666660108L, "Besos En Guerra", 193, "/users/storage/morat/audio/besos_en_guerra"));
    al9->agregarCancion(new Cancion(666660109L, "Como Te Atreves", 199, "/users/storage/morat/audio/como_te_atreves"));
    al9->agregarCancion(new Cancion(666660110L, "Mi Nuevo Vicio", 211, "/users/storage/morat/audio/mi_nuevo_vicio"));
    al9->agregarCancion(new Cancion(666660111L, "Aprender a Quererte", 205, "/users/storage/morat/audio/aprender_a_quererte"));
    totalCanciones += 11;

    a9->agregarAlbum(al9);
    gestorArtistas->agregarArtista(a9);

    // ========== ARTISTA 10: Bomba Estereo (ID: 77777) ==========
    Artista* a10 = new Artista(77777, 15, "Colombia");
    a10->setNombre("Bomba Estereo");
    a10->setCantidadSeguidores(3800000);
    a10->setPosicionTendencias(10);

    Album* al10 = new Album(1, "Deja", "2021-09-10", "Sony Music");
    al10->setRutaPortada("/users/storage/bombaestereo/image/deja.png");
    al10->agregarGenero("Electronica");
    al10->agregarGenero("Latina");

    // IDs: 77777 (artista) + 01 (álbum) + 01-10 (canciones)
    al10->agregarCancion(new Cancion(777770101L, "Fuego", 198, "/users/storage/bombaestereo/audio/fuego"));
    al10->agregarCancion(new Cancion(777770102L, "Soy Yo", 187, "/users/storage/bombaestereo/audio/soy_yo"));
    al10->agregarCancion(new Cancion(777770103L, "To My Love", 203, "/users/storage/bombaestereo/audio/to_my_love"));
    al10->agregarCancion(new Cancion(777770104L, "Ojitos Lindos", 215, "/users/storage/bombaestereo/audio/ojitos_lindos"));
    al10->agregarCancion(new Cancion(777770105L, "El Alma y El Cuerpo", 191, "/users/storage/bombaestereo/audio/el_alma_y_el_cuerpo"));
    al10->agregarCancion(new Cancion(777770106L, "Internacionales", 209, "/users/storage/bombaestereo/audio/internacionales"));
    al10->agregarCancion(new Cancion(777770107L, "Amar Asi", 197, "/users/storage/bombaestereo/audio/amar_asi"));
    al10->agregarCancion(new Cancion(777770108L, "Vuelo", 205, "/users/storage/bombaestereo/audio/vuelo"));
    al10->agregarCancion(new Cancion(777770109L, "Deja", 193, "/users/storage/bombaestereo/audio/deja"));
    al10->agregarCancion(new Cancion(777770110L, "Carnaval", 201, "/users/storage/bombaestereo/audio/carnaval"));
    totalCanciones += 10;

    a10->agregarAlbum(al10);
    gestorArtistas->agregarArtista(a10);

    // ------------------- Publicidad  ------------------
    // Categoría AAA (prioridad 3)
    gestorPublicidad->agregarMensaje(new MensajePublicitario("Descubre UdeATunes Premium: sin anuncios, mejor calidad, listas ilimitadas. Solo $19,900/mes", "AAA"));
    gestorPublicidad->agregarMensaje(new MensajePublicitario("Descarga la app movil de UdeATunes. Musica sin limites donde vayas", "AAA"));
    gestorPublicidad->agregarMensaje(new MensajePublicitario("Crea hasta 10,000 canciones en tu lista de favoritos con UdeATunes Premium", "AAA"));
    gestorPublicidad->agregarMensaje(new MensajePublicitario("Prueba gratis UdeATunes Premium por 30 dias. Cancela cuando quieras", "AAA"));
    gestorPublicidad->agregarMensaje(new MensajePublicitario("Escucha en calidad HD 320 kbps solo con Premium. Siente cada detalle de tu musica favorita", "AAA"));

    // Categoría B (prioridad 2)
    gestorPublicidad->agregarMensaje(new MensajePublicitario("Sigue a tus artistas favoritos y recibe notificaciones de nuevos lanzamientos", "B"));
    gestorPublicidad->agregarMensaje(new MensajePublicitario("Nueva musica colombiana todos los viernes. No te pierdas los ultimos lanzamientos", "B"));
    gestorPublicidad->agregarMensaje(new MensajePublicitario("Escucha en alta calidad con membresia Premium. Experimenta la diferencia", "B"));
    gestorPublicidad->agregarMensaje(new MensajePublicitario("Unete a la comunidad de melomanos mas grande de Colombia", "B"));
    gestorPublicidad->agregarMensaje(new MensajePublicitario("Comparte tus playlists con amigos y descubre nueva musica juntos", "B"));
    gestorPublicidad->agregarMensaje(new MensajePublicitario("Modo sin conexion disponible para Premium. Lleva tu musica a donde vayas", "B"));

    // Categoría C (prioridad 1)
    gestorPublicidad->agregarMensaje(new MensajePublicitario("Visita udeatunes.com para mas informacion sobre nuestros planes", "C"));
    gestorPublicidad->agregarMensaje(new MensajePublicitario("Conecta con millones de usuarios. Comparte tus playlists favoritas en UdeATunes", "C"));
    gestorPublicidad->agregarMensaje(new MensajePublicitario("Descubre playlists personalizadas basadas en tus gustos musicales", "C"));
    gestorPublicidad->agregarMensaje(new MensajePublicitario("Mas de 100 canciones de artistas colombianos disponibles en UdeATunes", "C"));
    gestorPublicidad->agregarMensaje(new MensajePublicitario("Siguenos en redes sociales para concursos y premios exclusivos", "C"));
    gestorPublicidad->agregarMensaje(new MensajePublicitario("Regala membresia Premium a tus amigos y familiares", "C"));

    // ---------------- Favoritos VARIADOS para usuarios premium ----------------

    // Arrays específicos para cada usuario premium con diferentes cantidades
    int favoritosPorUsuario[] = {7, 12, 5, 15, 9, 11}; // Diferentes cantidades para cada uno
    int indicePremium = 0;

    for (int ui = 0; ui < gestorUsuarios->getCantidadUsuarios(); ++ui) {
        Usuario* u = gestorUsuarios->obtenerPorIndice(ui);
        if (!u || !u->esPremium()) continue;

        int objetivoFavoritos = favoritosPorUsuario[indicePremium];
        indicePremium++;

        int agregadas = 0;
        int intentos = 0;

        // Asegurar variedad: usar diferentes rangos de artistas por usuario
        while (agregadas < objetivoFavoritos && intentos < 300) {
            Cancion* c = gestorArtistas->obtenerCancionAleatoria();
            if (c && u->agregarFavorito(c)) {
                agregadas++;
            }
            intentos++;
        }
    }

    // ====== GUARDAR AUTOMÁTICAMENTE DESPUÉS DE GENERAR ======
    gestorUsuarios->guardarEnArchivo("./data/usuarios.dat");
    gestorArtistas->guardarEnArchivo("./data/artistas.dat");
    gestorPublicidad->guardarEnArchivo("./data/publicidad.dat");

    cout << "\n==========================================        " << endl;
    cout << "         DATOS DE PRUEBA GENERADOS                  " << endl;
    cout << "  10 usuarios | 10 artistas | 109 canciones"          << endl;
    cout << "============================================      \n" << endl;

    //mostrarUsuariosDemo();
}

// -----------------------------------
//      Login simple por nickname
// -----------------------------------
bool Sistema::login() {
    string nickname;

    cout << "========================================" << endl;
    cout << "          INICIO DE SESION" << endl;
    cout << "========================================" << endl;
    cout << "Ingrese su nickname:";
    cin >> nickname;

    if (gestorUsuarios->validarLogin(nickname)) {
        usuarioActual = gestorUsuarios->buscarUsuario(nickname);
        cout << "\nBienvenido " << usuarioActual->getNickname() << endl;
        cout << "Tipo de membresia: " << usuarioActual->getTipoMembresia() << endl;
        return true;
    }
    cout << "Usuario no encontrado" << endl;
    return false;
}

// ---------------------------------
//           Menú principal
// ---------------------------------
void Sistema::menuPrincipal() {
    int opcion;

    while (sistemaActivo) {
        cout << "\n========================================" << endl;
        cout << "           MENU PRINCIPAL" << endl;
        cout << "========================================" << endl;
        cout << "Usuario: " << usuarioActual->getNickname()
             << " (" << usuarioActual->getTipoMembresia() << ")" << endl;
        cout << "========================================" << endl;
        cout << "1. Reproduccion aleatoria" << endl;

        if (usuarioActual->esPremium()) {
            cout << "2. Mi lista de favoritos" << endl;
            cout << "3. Pausar reproduccion" << endl;
            cout << "4. Salir" << endl;
        } else {
            cout << "2. Pausar reproduccion" << endl;
            cout << "3. Salir" << endl;
        }

        cout << "========================================" << endl;
        cout << "Seleccione una opcion:";
        cin >> opcion;

        if (!reproductor) reproductor = new Reproductor(usuarioActual, gestorArtistas, gestorPublicidad);

        if (usuarioActual->esPremium()) {
            switch (opcion) {
            case 1:
                MedidorRecursos::reiniciarContador();
                reproductor->reproduccionAleatoria();
                MedidorRecursos::calcularMemoriaTotal(this);
                MedidorRecursos::mostrarReporte();
                break;
            case 2:
                menuFavoritos();
                break;
            case 3:
                cout << "\nReproduccion pausada" << endl;
                break;
            case 4:
                finalizar();
                return;
            default:
                cout << "Opcion invalida" << endl;
            }
        } else {
            switch (opcion) {
            case 1:
                MedidorRecursos::reiniciarContador();
                reproductor->reproduccionAleatoria();
                MedidorRecursos::calcularMemoriaTotal(this);
                MedidorRecursos::mostrarReporte();
                break;
            case 2:
                cout << "\nReproduccion pausada" << endl;
                break;
            case 3:
                finalizar();
                return;
            default:
                cout << "Opcion invalida" << endl;
            }
        }
    }
}

// ---------------------------------------------------------------------
//      Submenú: Mi lista de favoritos (editar / seguir / ejecutar)
// ---------------------------------------------------------------------
void Sistema::menuFavoritos() {
    int op = 0;
    do {
        cout << "\n========================================" << endl;
        cout << "          MI LISTA DE FAVORITOS" << endl;
        cout << "========================================" << endl;
        cout << "1. Ver mi lista de favoritos" << endl;
        cout << "2. Editar mi lista de favoritos" << endl;
        cout << "3. Seguir otra lista de favoritos" << endl;
        cout << "4. Ejecutar mi lista de favoritos" << endl;
        cout << "5. Volver" << endl;
        cout << "========================================" << endl;
        cout << "Seleccione una opcion:";
        cin >> op;

        switch (op) {
        case 1:
            MedidorRecursos::reiniciarContador();
            verMisFavoritos();
            MedidorRecursos::calcularMemoriaTotal(this);
            MedidorRecursos::mostrarReporte();
            break;
        case 2:
            MedidorRecursos::reiniciarContador();
            editarFavoritos();
            gestorUsuarios->guardarEnArchivo("./data/usuarios.dat");
            MedidorRecursos::calcularMemoriaTotal(this);
            MedidorRecursos::mostrarReporte();
            break;
        case 3:
            MedidorRecursos::reiniciarContador();
            seguirOtraLista();
            gestorUsuarios->guardarEnArchivo("./data/usuarios.dat");
            MedidorRecursos::calcularMemoriaTotal(this);
            MedidorRecursos::mostrarReporte();
            break;
        case 4: {
            int modo = 0;
            cout << "\n1. Secuencial\n2. Aleatorio\nSeleccione el modo: ";
            cin >> modo;
            bool aleatorio = (modo == 2);
            int tipoReproduccion = 0;
            cout << "\n1. Control manual (una por una)\n2. Reproduccion automatica completa\nSeleccione: ";
            cin >> tipoReproduccion;

            if (!reproductor) reproductor = new Reproductor(usuarioActual, gestorArtistas, gestorPublicidad);

            MedidorRecursos::reiniciarContador();

            if (tipoReproduccion == 1) {
                reproductor->reproducirFavoritos(aleatorio);
            } else {
                reproductor->reproducirFavoritosAutomatico(aleatorio);
            }

            MedidorRecursos::calcularMemoriaTotal(this);
            MedidorRecursos::mostrarReporte();
            break;
        }
        case 5:
            break;
        default:
            cout << "Opcion invalida" << endl;
        }
    } while (op != 5);
}

void Sistema::verMisFavoritos() {
    if (!usuarioActual || !usuarioActual->esPremium()) {
        cout << "Esta funcion es solo para usuarios premium." << endl;
        return;
    }

    ListaFavoritos* fav = usuarioActual->getFavoritos();
    if (!fav || fav->getCantidad() == 0) {
        cout << "\nNo tienes canciones en favoritos." << endl;
        return;
    }

    cout << "\n========================================" << endl;
    cout << "        MI LISTA DE FAVORITOS" << endl;
    cout << "========================================" << endl;
    cout << "Total de canciones: " << fav->getCantidad() << endl;
    cout << "========================================" << endl;

    for (int i = 0; i < fav->getCantidad(); ++i) {
        Cancion* c = fav->obtenerCancion(i);
        if (!c) continue;

        Album* alb = c->getAlbum();
        Artista* art = (alb && gestorArtistas) ? gestorArtistas->obtenerArtistaPorAlbum(alb) : nullptr;

        cout << (i + 1) << ". ";
        cout << "ID: " << c->getIdentificador() << " | ";
        cout << (art ? art->getNombre() : "Desconocido") << " - ";
        cout << c->getNombre() << endl;

        MedidorRecursos::incrementarIteraciones();
    }

    cout << "========================================\n" << endl;
}

// ---------------------------------------------------------------------
//              Editar favoritos (agregar / eliminar por ID)
// ---------------------------------------------------------------------
void Sistema::editarFavoritos() {
    long id;
    int accion;

    cout << "\n========================================" << endl;
    cout << "        EDITAR FAVORITOS" << endl;
    cout << "========================================" << endl;
    cout << "Ingrese el ID de la cancion (9 digitos): ";
    cin >> id;

    //Buscar canción en el catálogo
    Cancion* encontrada = 0;
    int nA = gestorArtistas->getCantidad();
    for (int i = 0; i < nA && !encontrada; ++i) {
        MedidorRecursos::incrementarIteraciones();
        Artista* ar = gestorArtistas->obtener(i);
        if (!ar) continue;
        int nAl = ar->getCantidadAlbumes();
        for (int j = 0; j < nAl && !encontrada; ++j) {
            MedidorRecursos::incrementarIteraciones();
            Album* al = ar->obtenerAlbum(j);
            if (!al) continue;
            encontrada = al->buscarCancion(id);
        }
    }

    if (!encontrada) {
        cout << "Cancion no encontrada" << endl;
        return;
    }

    cout << "\nCancion: " << encontrada->getNombre() << endl;
    cout << "1. Agregar a favoritos" << endl;
    cout << "2. Eliminar de favoritos" << endl;
    cout << "Seleccione una accion: ";
    cin >> accion;

    bool cambioRealizado = false;

    if (accion == 1) {
        if (usuarioActual->agregarFavorito(encontrada)) {
            cout << "Cancion agregada a favoritos" << endl;
            cambioRealizado = true;
        } else {
            cout << "No se pudo agregar (evitar duplicados o limite)" << endl;
        }
    } else if (accion == 2) {
        if (usuarioActual->eliminarFavorito(id)) {
            cout << "Cancion eliminada de favoritos" << endl;
            cambioRealizado = true;
        } else {
            cout << "No se pudo eliminar (puede no estar en favoritos)" << endl;
        }
    }

    // ========== GUARDAR INMEDIATAMENTE SI HUBO CAMBIOS ==========
    if (cambioRealizado) {
        cout << "Guardando cambios..." << endl;
        if (gestorUsuarios->guardarEnArchivo("data/usuarios.dat")) {
            cout << "Cambios guardados exitosamente." << endl;
        } else {
            cout << "ERROR: No se pudieron guardar los cambios." << endl;
        }
    }
}

// -------------------------------------------------------------------------------------
//          Seguir otra lista (combina respetando reglas del gestor de usuarios)
// -------------------------------------------------------------------------------------
void Sistema::seguirOtraLista() {
    string nick;
    cout << "\n========================================" << endl;
    cout << "       SEGUIR LISTA DE FAVORITOS" << endl;
    cout << "========================================" << endl;
    cout << "Ingrese el nickname del usuario: ";
    cin >> nick;

    Usuario* otro = gestorUsuarios->buscarUsuario(nick);
    MedidorRecursos::incrementarIteraciones();

    if (!otro) {
        cout << "Usuario no encontrado" << endl;
        return;
    }
    if (!otro->esPremium()) {
        cout << "El usuario no es premium" << endl;
        return;
    }

    if (usuarioActual->seguirListaOtroUsuario(otro)) {
        cout << "Lista de favoritos combinada exitosamente" << endl;

        // ========== GUARDAR INMEDIATAMENTE ==========
        cout << "Guardando cambios..." << endl;
        if (gestorUsuarios->guardarEnArchivo("data/usuarios.dat")) {
            cout << "Cambios guardados exitosamente." << endl;
        } else {
            cout << "ERROR: No se pudieron guardar los cambios." << endl;
        }
    } else {
        cout << "No se pudo combinar la lista" << endl;
    }
}

// ---------------------------------------------------------------------
//                          Flujo principal
// ---------------------------------------------------------------------
void Sistema::iniciar() {
    cout << "========================================        " << endl;
    cout << "          BIENVENIDO A UDEATUNES               " << endl;
    cout << "     Sistema de Streaming Musical               " << endl;
    cout << "========================================        \n" << endl;

    cargarDatos();
    mostrarUsuariosDemo();

    while (!login()) {
        cout << "\nIntente nuevamente" << endl;
    }

    menuPrincipal();
}

void Sistema::finalizar() {
    cout << "\n========================================" << endl;
    cout << "         GUARDANDO DATOS..." << endl;
    cout << "========================================" << endl;

    gestorUsuarios->guardarEnArchivo("./data/usuarios.dat");
    gestorArtistas->guardarEnArchivo("./data/artistas.dat");
    gestorPublicidad->guardarEnArchivo("./data/publicidad.dat");

    cout << "Datos guardados correctamente" << endl;
    cout << "Gracias por usar UdeATunes" << endl;
    cout << "========================================\n" << endl;

    sistemaActivo = false;
}

// ---------------------------------------------------------------------
//                   Soporte de métricas/medición
// ---------------------------------------------------------------------
GestorUsuarios*  Sistema::getGestorUsuarios()  const { return gestorUsuarios; }
GestorArtistas*  Sistema::getGestorArtistas()  const { return gestorArtistas; }
GestorPublicidad* Sistema::getGestorPublicidad() const { return gestorPublicidad; }

long Sistema::calcularMemoriaTotal() const {
    long m = sizeof(Sistema);
    if (gestorUsuarios)   m += gestorUsuarios->calcularMemoria();
    if (gestorArtistas)   m += gestorArtistas->calcularMemoria();
    if (gestorPublicidad) m += gestorPublicidad->calcularMemoria();
    if (reproductor)      m += reproductor->calcularMemoria();
    return m;
}
