{
  TCanvas *c1= new TCanvas("c1");

  TH1F *h[10];
  h[1] = hNnp01xtn;

  TH1F *h2 = new TH1F((*h[1])+(*hNnp01xon));
  h[2] = h2;
  TH1F *h3 = new TH1F((*h[2])+(*hNnp01xap));
  h[3] = h3;
  TH1F *h4 = new TH1F((*h[3])+(*hNnp01xadt));
  h[4] = h4;
  TH1F *h5 = new TH1F((*h[4])+(*hNnp01xahe));
  h[5] = h5;
  TH1F *h6 = new TH1F((*h[5])+(*hNnp01xag));
  h[6] = h6;
  TH1F *h7 = new TH1F((*h[6])+(*hNnp01xapm));
  h[7] = h7;
  TH1F *hx;
  hx= hNnp01x;

  hx->SetFillColor(0);
  hx->SetFillStyle(3001);
  //  hx->SetAxisRange(0., 5000.,"Y");
  hx->Draw();

  for (Int_t ih=7; ih>0; ih--) { 
    h[ih]->SetFillColor(ih);
    h[ih]->SetFillStyle(3001);
    h[ih]->Draw("Same");
  }

  c1->Print(Form("plot_tLand.png"),"png");
  //  c1->Print(Form("plot_tLand.range.png"),"png");
  
}
