# ttH



### How to access a Jupyter Notebook from lxplus?

1. (Optional) Create a virtual environment on lxplus:

   ```
   virtualenv jupyter_notebook_env
   source jupyter_notebook_env/bin/activate
   pip install --upgrade pip
   pip install jupyter
   ```

   `xrootd` would be needed to access remotely data and mc root files
   from CERN cluster

   ```
   pip install xrootd
   pip install uproot
   pip install pandas
   ```

   Set up `voms proxy` if needed:

   ```
   voms-proxy-init --voms cms
   ```

   Run jupyter (use an available port)

   ```
   jupyter notebook --no-browser --port:5432 &
   ```

2. Create a ssh local tunnel to lxplus, asuming you were connected to `lxplus749`:

   ```
   ssh -L 5432:localhost:5432 username@lxplus749.cern.ch
   ```

   Then, you can access locally by using the link provided by jupyter in your local
   machine (no need to setup a browser proxy).


### How to open CMS root-like files using root?

To be able to read CMSSW objects directly from ROOT, we would need
to load FWLiteCore:

```
gSystem->Load("libFWCoreFWLite.so");
FWLiteEnabler::enable();
gSystem->Load("libDataFormatsFWLite.so");
gSystem->Load("libDataFormatsPatCandidates.so");
```

Note: This requires `cmsenv`.