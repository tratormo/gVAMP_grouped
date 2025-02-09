#pragma once
#include <string>
#include <vector>
#include <memory>
#include <immintrin.h> // contains definition of _mm_malloc


// class data contains genotypic and phenotypic data and auxiliary functions related
// to the loading of genotypic and phenotypic data as well as computing marker statistics 
// and efficient dot product between a marker and a phenotype vector.

class data {

private:

    int Mt;     // total number of markers
    int N;          // number of individuals 
    int M;          // number of markers attributed to the node
    int K;          // number of groups
    int S;          // marker starting index 
    int rank;       // rank of MPI process
    // int C = 0;      // number of covariates (in a probit model)

    std::string phenfp;       // filepath to phenotypes
    std::string bedfp;          // filepath to .bed file
    std::string methfp;          // filepath to methylation data
    std::string bimfp;           // filepath to .bim file
    std::string groupfp;          // filepath to .txt group assignment
    // std::string covfp;             // filepath to covariates (in a probit model)
    std::vector<double> phen_data;   // vector of phenotype data
    std::string type_data;           // type of data used in inference("bed" or "meth")
    std::vector<int> group_assignments; // group_assignment for each entry

    int nonas;
    int nas;
    int m4;
    int im4 = 0;

    std::vector<unsigned char> mask4;

    double* mave     = nullptr;
    double* msig     = nullptr;

    std::vector<double> mave_people;
    std::vector<double> msig_people;
    std::vector<double> numb_people;

    unsigned char* bed_data = nullptr;
    double* meth_data = nullptr;
    std::vector< std::vector<double> > covs;

    size_t mbytes;

    double sigma_max = 1e8;
    double intercept = 0;
    double scale = 1; 
    double alpha_scale;

public:

    //***************************
    // ACCESSING CLASS VARIABLES
    //***************************
    std::vector<double> get_phen(){ return phen_data; };
    std::vector<int> get_group_assignments() { return group_assignments; }
    std::string get_bimfp() { return bimfp; }
    double get_intercept() { return intercept; };
    double get_scale() { return scale; };
    double get_sigma_max() { return sigma_max; };
    size_t get_mbytes(){ return mbytes; };

    unsigned char * get_bed_data() { return bed_data; }
    double * get_meth_data() { return meth_data; }
    std::vector< std::vector<double> > get_covs() { return covs; }

    void set_phen(std::vector<double> new_data) { phen_data = new_data; }

    double* get_mave()        { return mave; }
    double* get_msig()        { return msig; }

    std::vector<double> get_mave_people() { return mave_people; };
    std::vector<double> get_msig_people() { return msig_people; };
    std::vector<double> get_numb_people() { return numb_people; };

    std::vector<unsigned char>& get_mask4() { return mask4; }
    int     get_im4()   const { return im4; }
    int     get_nonas()       { return nonas; }
    void    set_nonas(int num){ nonas = num; }
    int     get_S()     const { return S; }
    int     get_K()     const { return K; }

    std::string get_type_data() const { return type_data; }

    
    //******************************
    //  CONSTRUCTORS and DESTRUCTOR
    //******************************
    data(std::vector<double> y, std::string genofp, std::string groupfp, const int N, const int M, const int Mt, int K, const int S, const int rank, std::string type_data = "bed", double alpha_scale = 1, std::string bimfp = "");
    data(std::string fp, std::string genofp, std::string groupfp, const int N, const int M, const int Mt, int K, const int S, const int rank, std::string type_data = "bed", double alpha_scale = 1, std::string bimfp = "");
    ~data() {
        if (mave     != nullptr)  _mm_free(mave);
        if (msig     != nullptr)  _mm_free(msig);
    }
    

    //**************************
    // DATA LOADING PROCEDURES
    //**************************
    void read_phen();   // reading phenotype file
    void read_genotype_data();
    void read_group_assignments();
    void read_covariates(std::string covfp, int C = 0);
    void read_methylation_data();
    std::vector<int> read_chromosome_info(std::string bim_file);

    void compute_markers_statistics(); 
    void compute_people_statistics();


    //**************************************************************
    // PROCEDURES IMPLEMENTING OPERATIONS INVOLVING DESIGN MATRIX A
    //**************************************************************
    std::vector<double> Ax(double* __restrict__ phen, int SB, int LB);
    std::vector<double> Ax(double* __restrict__ phen);
    
    std::vector<double> ATx(double* __restrict__ phen, int SB, int LB);
    std::vector<double> ATx(double* __restrict__ phen);
    double dot_product(const int mloc, double* __restrict__ phen, const double mu, const double sigma_inv, const int SB, const int LB);

    std::vector<double> Zx(std::vector<double> phen);



    //****************************
    // FILTERING PHENOTYPE VALUES
    //*****************************
    std::vector<double> filter_pheno();
    std::vector<double> filter_pheno(int *nonnan);
    

        
    //*****************************************************
    // CALCULATION OF p-values using LOO and LOCO APPROACH
    //*****************************************************
    std::vector< std::vector<double> > pvals_calc(std::vector< std::vector<double> > z1, std::vector<double> y, std::vector< std::vector<double> > x1_hat, std::vector< std::string > filepath);
    std::vector< std::vector<double> > pvals_calc_LOCO(std::vector< std::vector<double> > z1, std::vector<double> y, std::vector< std::vector<double> > x1_hat, std::vector< std::string > filepath);

    

};


