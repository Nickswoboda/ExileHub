#include "app.hpp"

App::App(const QString& executable_path) : executable_path_(executable_path) {}

App::App(const QString& name, const QString& executable_path)
    : name_(name), executable_path_(executable_path)
{
}
