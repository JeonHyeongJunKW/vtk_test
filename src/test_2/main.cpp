#include <vtkSmartPointer.h>
#include <vtkRenderWindow.h>
#include <vtkRenderer.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkProperty.h>
#include <vtkPolyData.h>
#include <vtkPoints.h>
#include <vtkVertexGlyphFilter.h>
#include <vtkPolyDataMapper.h>
#include <vtkActor.h>
#include <vtkCamera.h>
#include <vtkCallbackCommand.h>
#include <vtkCommand.h>


class PointCloudUpdater {
public:
  vtkSmartPointer<vtkPolyData> polyData_;

  void UpdatePointCloud() {
    auto points = vtkSmartPointer<vtkPoints>::New();

    static double t = 0;
    t += 0.1;

    // 예시: 동적으로 움직이는 원
    for (int i = 0; i < 100; ++i) {
      double angle = 2.0 * 3.14159 * i / 100;
      double x = cos(angle + t);
      double y = sin(angle + t);
      double z = 0.5 * sin(t + i * 0.1);
      points->InsertNextPoint(x, y, z);
    }

    polyData_->SetPoints(points);
    polyData_->Modified();
  }
};

void TimerCallback(vtkObject* caller, unsigned long, void* clientData, void*) {
  auto iren = static_cast<vtkRenderWindowInteractor*>(caller);
  auto updater = static_cast<PointCloudUpdater*>(clientData);

  updater->UpdatePointCloud();
  iren->GetRenderWindow()->Render();
}

int main() {
  auto updater = new PointCloudUpdater();
  updater->polyData_ = vtkSmartPointer<vtkPolyData>::New();

  // 포인트들을 렌더링하기 위해 GlyphFilter 사용
  auto glyphFilter = vtkSmartPointer<vtkVertexGlyphFilter>::New();
  glyphFilter->SetInputData(updater->polyData_);
  glyphFilter->Update();

  auto mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
  mapper->SetInputConnection(glyphFilter->GetOutputPort());

  auto actor = vtkSmartPointer<vtkActor>::New();
  actor->SetMapper(mapper);
  actor->GetProperty()->SetPointSize(5);

  auto renderer = vtkSmartPointer<vtkRenderer>::New();
  renderer->AddActor(actor);
  renderer->SetBackground(0.1, 0.1, 0.1);

  auto camera = renderer->GetActiveCamera();
  camera->SetPosition(0.0, 0.0, 10.0);
  camera->SetFocalPoint(0.0, 0.0, 0.0);
  camera->SetViewUp(0.0, 1.0, 0.0);

  auto renderWindow = vtkSmartPointer<vtkRenderWindow>::New();
  renderWindow->AddRenderer(renderer);

  auto interactor = vtkSmartPointer<vtkRenderWindowInteractor>::New();
  interactor->SetRenderWindow(renderWindow);

  auto timerCallback = vtkSmartPointer<vtkCallbackCommand>::New();
  timerCallback->SetCallback(TimerCallback);
  timerCallback->SetClientData(updater);

  interactor->AddObserver(vtkCommand::TimerEvent, timerCallback);
  interactor->Initialize();
  interactor->CreateRepeatingTimer(30);  // 30ms마다 업데이트
  renderWindow->Render();
  interactor->Start();

  delete updater;
  return 0;
}
