#include "graph_controller.h"
#include "../app.h"
#include <assert.h>
#include <math.h>

namespace Graph {

GraphController::GraphController(Responder * parentResponder, FunctionStore * functionStore, HeaderViewController * header) :
  ViewController(parentResponder),
  HeaderViewDelegate(header),
  m_view(GraphView(functionStore, &m_graphWindow)),
  m_graphWindow(functionStore),
  m_windowParameterController(WindowParameterController(this, &m_graphWindow, &m_view)),
  m_zoomParameterController(ZoomParameterController(this, &m_graphWindow, &m_view)),
  m_initialisationParameterController(InitialisationParameterController(this, &m_graphWindow, &m_view)),
  m_curveParameterController(CurveParameterController(&m_view)),
  m_windowButton(this, "Axes", Invocation([](void * context, void * sender) {
    GraphController * graphController = (GraphController *) context;
    StackViewController * stack = graphController->stackController();
    stack->push(graphController->windowParameterController());
  }, this)),
  m_zoomButton(this, "Zoom", Invocation([](void * context, void * sender) {
    GraphController * graphController = (GraphController *) context;
    StackViewController * stack = graphController->stackController();
    stack->push(graphController->zoomParameterController());
  }, this)),
  m_defaultInitialisationButton(this, "Initialisation", Invocation([](void * context, void * sender) {
    GraphController * graphController = (GraphController *) context;
    StackViewController * stack = graphController->stackController();
    stack->push(graphController->initialisationParameterController());
  }, this)),
  m_functionStore(functionStore)
{
}

View * GraphController::view() {
  if (m_view.xPixelCursorPosition() < 0.0f && !isEmpty()) {
    m_view.initCursorPosition();
  }
  return &m_view;
}

const char * GraphController::title() const {
  return "Graphique";
}

bool GraphController::isEmpty() {
  if (m_functionStore->numberOfActiveFunctions() == 0) {
    return true;
  }
  return false;
}

const char * GraphController::emptyMessage() {
  if (m_functionStore->numberOfDefinedFunctions() == 0) {
    return "Aucune fonction";
  }
  return "Aucune fonction selectionnee";
}

Responder * GraphController::defaultController() {
  return tabController();
}

Responder * GraphController::tabController() const{
  return (parentResponder()->parentResponder()->parentResponder()->parentResponder());
}

StackViewController * GraphController::stackController() const{
  return (StackViewController *)(parentResponder()->parentResponder()->parentResponder());
}

ViewController * GraphController::windowParameterController() {
  return &m_windowParameterController;
}

ViewController * GraphController::zoomParameterController() {
  return &m_zoomParameterController;
}

ViewController * GraphController::initialisationParameterController() {
  return &m_initialisationParameterController;
}

int GraphController::numberOfButtons() const {
  return 3;
}

Button * GraphController::buttonAtIndex(int index) {
  switch (index) {
    case 0:
      return &m_windowButton;
    case 1:
      return &m_zoomButton;
    case 2:
      return &m_defaultInitialisationButton;
    default:
      assert(false);
  }
  return nullptr;
}

void GraphController::didBecomeFirstResponder() {
  if (m_view.context() == nullptr) {
    App * graphApp = (Graph::App *)app();
    m_view.setContext(graphApp->evaluateContext());
  }
  if (m_graphWindow.context() == nullptr) {
    App * graphApp = (Graph::App *)app();
    m_graphWindow.setContext(graphApp->evaluateContext());
  }
  // if new functions were added to the store, the window parameters need to be refresh
  m_graphWindow.computeYaxes();
  headerViewController()->setSelectedButton(-1);
  m_headerSelected = false;

  m_view.setVisibleCursor(true);
  // Layout view whe the graph view that might have been modified by the zoom page
  headerViewController()->layoutView();
  // Reload graph view
  m_view.reload();
}

bool GraphController::handleEvent(Ion::Events::Event event) {
  if (m_headerSelected) {
    if (event == Ion::Events::Down) {
        headerViewController()->setSelectedButton(-1);
        m_headerSelected = false;
        app()->setFirstResponder(this);
        m_view.setVisibleCursor(true);
        return true;
    }
    if (event == Ion::Events::Up) {
      headerViewController()->setSelectedButton(-1);
      app()->setFirstResponder(tabController());
    }
    return headerViewController()->handleEvent(event);
  } else {
    if (event == Ion::Events::Plus) {
      float xMin = m_graphWindow.xMin();
      float xMax = m_graphWindow.xMax();
      float yMin = m_graphWindow.yMin();
      float yMax = m_graphWindow.yMax();
      m_graphWindow.setXMin((xMax+xMin)/2.0f - fabsf(xMax-xMin)/3.0f);
      m_graphWindow.setXMax((xMax+xMin)/2.0f + fabsf(xMax-xMin)/3.0f);
      m_graphWindow.setYAuto(false);
      m_graphWindow.setYMin((yMax+yMin)/2.0f - fabsf(yMax-yMin)/3.0f);
      m_graphWindow.setYMax((yMax+yMin)/2.0f + fabsf(yMax-yMin)/3.0f);
      m_view.initCursorPosition();
      m_view.reload();
      return true;
    }
    if (event == Ion::Events::Minus) {
      float xMin = m_graphWindow.xMin();
      float xMax = m_graphWindow.xMax();
      float yMin = m_graphWindow.yMin();
      float yMax = m_graphWindow.yMax();
      m_graphWindow.setXMin((xMax+xMin)/2.0f - 3.0f*fabsf(xMax-xMin)/4.0f);
      m_graphWindow.setXMax((xMax+xMin)/2.0f + 3.0f*fabsf(xMax-xMin)/4.0f);
      m_graphWindow.setYAuto(false);
      m_graphWindow.setYMin((yMax+yMin)/2.0f - 3.0f*fabsf(yMax-yMin)/4.0f);
      m_graphWindow.setYMax((yMax+yMin)/2.0f + 3.0f*fabsf(yMax-yMin)/4.0f);
      m_view.initCursorPosition();
      m_view.reload();
      return true;
    }
    if (event == Ion::Events::Left) {
      m_view.moveCursorHorizontally(-2);
      return true;
    }
    if (event == Ion::Events::Right) {
      m_view.moveCursorHorizontally(2);
      return true;
    }
    if (event == Ion::Events::Up) {
      Function * f = m_view.moveCursorUp();
      if (f == nullptr) {
        m_view.initCursorPosition();
        m_view.setVisibleCursor(false);
        headerViewController()->setSelectedButton(0);
        m_headerSelected = true;
      }
      return true;
    }
    if (event == Ion::Events::Down) {
      Function * f = m_view.moveCursorDown();
      if (f == nullptr) {
        return false;
      }
      return true;
    }
    if (event == Ion::Events::OK) {
      Function * f = m_functionStore->activeFunctionAtIndex(m_view.indexFunctionSelectedByCursor());
      m_curveParameterController.setFunction(f);
      StackViewController * stack = stackController();
      stack->push(&m_curveParameterController);
    }
    return false;
  }
}

}
