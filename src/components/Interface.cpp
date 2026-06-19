#include <components/Interface.hpp>

/* ========================================================================== */
/*                         CONSTRUCTOR AND DESTRUCTOR                         */
/* ========================================================================== */

Interface::Interface(void) : window(nullptr), io(nullptr), _isInit(false) {}

Interface::~Interface() { this->shutdown(); }

/* ========================================================================== */
/*                                    INIT                                    */
/* ========================================================================== */

void Interface::init(Window& window) {
  this->window = &window;
  _isInit = true;

  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  io = &ImGui::GetIO();
  io->ConfigFlags |= ImGuiConfigFlags_DockingEnable;
  ImGui::StyleColorsDark();
  this->window->initImGui();
  ImGui_ImplOpenGL3_Init("#version 460");
}

/* ========================================================================== */
/*                                   UPDATE                                   */
/* ========================================================================== */

void Interface::render(void) const {
  ImGui::Render();
  ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

bool Interface::wantCaptureMouse(void) const { return io->WantCaptureMouse; }

void Interface::createFrame(void) const {
  ImGui_ImplGlfw_NewFrame();
  ImGui_ImplOpenGL3_NewFrame();
  ImGui::NewFrame();
  ImGui::DockSpaceOverViewport(0, ImGui::GetMainViewport(),
                               ImGuiDockNodeFlags_PassthruCentralNode);
}

/* ========================================================================== */
/*                                   DESTROY                                  */
/* ========================================================================== */

void Interface::shutdown(void) {
  if (_isInit) {
    ImGui_ImplGlfw_Shutdown();
    ImGui_ImplOpenGL3_Shutdown();
    ImGui::DestroyContext();
    _isInit = false;
  }
}
