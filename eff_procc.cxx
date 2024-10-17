
#include "Core/JEFW.h"
#include "Core/JEfficiency.h"

#include <memory>


using std::unique_ptr;

void eff_procc(){


  unique_ptr<JEFW> mcjet{new JEFW("../../mcjetSpectraESE/AnalysisResults.root")};
  mcjet->Init("mc");
  auto hmatched = mcjet->GetHistPt(2);
  auto htruth = mcjet->GetHistPt(1);

  // divide mathced by truth to get efficiency
  hmatched->Divide(htruth);

  TCanvas c2("c2","",800,600);
  c2.cd();

  hmatched->SetTitle("");
  hmatched->GetYaxis()->SetTitle("Efficiency");
  hmatched->GetXaxis()->SetTitle("p_{T,jet}^{truth} (GeV/c)");

  hmatched->SetStats(0);
  hmatched->SetLineColor(kBlack);
  hmatched->Draw();

  hmatched->GetXaxis()->SetRangeUser(0, 120);
  hmatched->GetYaxis()->SetRangeUser(0, 1.1);

  // draw dashed line at 1
  TLine *line = new TLine(0, 1, 120, 1);
  line->SetLineStyle(2);
  line->Draw();

  c2.SaveAs("figures/efficiency.pdf");

}
