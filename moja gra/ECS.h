#pragma once // Zapobiega wielokrotnemu do³¹czaniu tego pliku nag³ówkowego

#include <iostream>
#include <vector>
#include <memory>
#include <algorithm>
#include <bitset>
#include <array>
#include <map>
#include "SDL.h"
#include "AlgorithSAT.h"

using namespace std;

// Deklaracje klas u¿ywanych w systemie ECS (Entity-Component-System)
class Component;
class Entity;
class Systems;
class Manager;

// Definiowanie typu identyfikatora komponentu
using ComponentID = size_t;

// Funkcja zwracaj¹ca unikalny identyfikator dla typu komponentu
inline ComponentID getComponentTypeID()
{
    static ComponentID lastID = 0; // Przechowywanie ostatniego ID
    return lastID++;
}

// Szablonowa wersja funkcji dla uzyskania identyfikatora komponentu konkretnego typu
template <typename T> inline ComponentID getComponentTypeID() noexcept
{
    static ComponentID typeID = getComponentTypeID(); // Tworzenie ID dla nowego typu komponentu
    return typeID;
}

// Maksymalna liczba komponentów, jakie mo¿e mieæ pojedyncza jednostka
constexpr size_t maxComponents = 32;

// Struktury przechowuj¹ce dane komponentów
using ComponentBitSet = bitset<maxComponents>; // Mapa bitowa reprezentuj¹ca obecnoœæ komponentów
using ComponentArray = array<Component*, maxComponents>; // Tablica wskaŸników na komponenty

// Bazowa klasa komponentów, z metodami inicjalizacji, aktualizacji i rysowania
class Component
{
public:
    Entity* entity; // WskaŸnik do jednostki, do której nale¿y komponent

    virtual void init() {} // Metoda inicjalizacji komponentu
    virtual void update() {} // Metoda aktualizacji komponentu w ka¿dej klatce
    virtual void draw() {} // Metoda rysowania komponentu

    virtual ~Component() {} // Wirtualny destruktor
};

// Klasa jednostki (Entity), która posiada komponenty
class Entity
{
private:
    bool active = true; // Status aktywnoœci jednostki
    bool isPlayer = false;
    bool isEnemy = false;
    vector<unique_ptr<Component>> components; // Wektor przechowuj¹cy komponenty danej jednostki

    ComponentArray componentArray; // Tablica wskaŸników na komponenty
    ComponentBitSet componentBitSet; // Mapa bitowa okreœlaj¹ca obecnoœæ komponentów
public:
    // Aktualizowanie wszystkich komponentów jednostki
    void update()
    {
        for (auto& c : components) c->update();
    }

    void setIsPlayer()
    {
        isPlayer = true;
    }

    bool getIsPlayer() { return isPlayer; }

    void setIsEnemy()
    {
        isEnemy = true;
    }

    bool getIsEnemy() { return isEnemy; }

    // Rysowanie wszystkich komponentów jednostki
    void draw()
    {
        for (auto& c : components) c->draw();
    }

    // Sprawdzanie aktywnoœci jednostki
    bool isActive() { return active; }

    // Dezaktywacja jednostki
    void destroy() { active = false; }

    // Sprawdzenie czy jednostka posiada dany komponent
    template <typename T> bool hasComponent() const
    {
        return componentBitSet[getComponentTypeID<T>()];
    }

    // Dodawanie komponentu do jednostki
    template <typename T, typename... TArgs>
    T& addComponent(TArgs&&...mArgs)
    {
        if (hasComponent<T>())
        {
            throw runtime_error("Component already exists"); // B³¹d, jeœli komponent ju¿ istnieje
        }

        T* c(new T(forward<TArgs>(mArgs)...)); // Tworzenie nowego komponentu
        c->entity = this; // Ustawienie jednostki dla komponentu
        unique_ptr<Component> uPtr{ c };
        components.emplace_back(move(uPtr)); // Przeniesienie do listy komponentów

        componentArray[getComponentTypeID<T>()] = c; // Przechowywanie wskaŸnika do komponentu
        componentBitSet[getComponentTypeID<T>()] = true; // Oznaczenie komponentu jako obecnego

        c->init(); // Inicjalizacja komponentu
        return *c;
    }

    // Pobieranie komponentu danego typu
    template <typename T> T& getComponent() const
    {
        if (!hasComponent<T>())
        {
            throw runtime_error("Component does not exist"); // B³¹d, jeœli komponent nie istnieje
        }

        auto ptr(componentArray[getComponentTypeID<T>()]);
        return *static_cast<T*>(ptr); // Konwersja wskaŸnika na typ komponentu
    }
};

// Klasa systemów ECS
class Systems {
public:
    Systems() {}

    // Aktualizacja systemów
    void update(Manager& manager, SDL_Renderer* ren, float deltaTime, float speed, const Uint8* keys, Uint32 mouseButtons, float mouseX, float mouseY)
    {
        movementSystem(manager, deltaTime, speed, keys, ren);
        atackSystem(manager, mouseButtons, mouseX, mouseY);
        collisionSystem(manager);
    }

    // Deklaracje systemów obs³uguj¹cych ró¿ne mechaniki gry
    static void movementSystem(Manager& manager, float deltaTime, float speed, const Uint8* keys, SDL_Renderer* ren);
    static void renderingSystem(Manager& manager, SDL_Renderer* ren);
    static void atackSystem(Manager& manager, Uint32 mouseButtons, float mouseX, float mouseY);
    static void collisionSystem(Manager& manager);
};

// Klasa zarz¹dzaj¹ca jednostkami i systemami
class Manager
{
private:
    Systems system;
    vector<unique_ptr<Entity>> entities; // Lista wszystkich jednostek
public:
    // Aktualizacja jednostek i systemów
    void update(Manager& manager, SDL_Renderer* ren, float deltaTime, float speed, const Uint8* keys, Uint32 mouseButtons, float mouseX, float mouseY)
    {
        system.update(manager, ren, deltaTime, speed, keys, mouseButtons, mouseX, mouseY);
        for (auto& e : entities) e->update();
    }

    // Rysowanie jednostek i systemów
    void draw(Manager& manager, SDL_Renderer* ren)
    {
        system.renderingSystem(manager, ren);
        for (auto& e : entities) e->draw();
    }

    // Usuwanie nieaktywnych jednostek
    void refresh()
    {
        entities.erase(remove_if(begin(entities), end(entities),
            [](const unique_ptr<Entity>& mEntity)
            {
                return !mEntity->isActive();
            }),
            end(entities));
    }

    // Tworzenie nowej jednostki
    Entity& addEntity()
    {
        Entity* e = new Entity();
        unique_ptr<Entity> uPtr{ e };
        entities.emplace_back(move(uPtr));
        return *e;
    }

    // Pobieranie listy jednostek
    vector<unique_ptr<Entity>>& getVectorOfEntities() { return entities; }

};
