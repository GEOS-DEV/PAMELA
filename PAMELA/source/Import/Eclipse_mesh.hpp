#pragma once
#include <vector>
#include "Mesh/UnstructuredMesh.hpp"
#include "Utils/Binary.hpp"
#include "Utils/File.hpp"


namespace PAMELA
{

  enum class ECLIPSE_MESH_TYPE { VERTEX, EDGE, QUADRILATERAL, HEXAHEDRON };
  enum class UNITS {FIELD,LAB,METRIC, UNKNOWN};

  class Eclipse_mesh
  {
    public:
      Eclipse_mesh() = default;
      Mesh* CreateMeshFromGRDECL(File file);
      Mesh* CreateMeshFromEclipseBinaryFiles(File file);

    private:
      void ParseStringFromGRDECL(std::string& str);
      std::string ConvertFiletoString(File file);
      void ParseStringFromBinaryFile(std::string& str);
      std::string extractDataBelowKeyword(std::istringstream& string_block);
      Mesh* ConvertMesh();
      void FillMeshWithProperties(Mesh* mesh);


      std::string m_label;

      ///Eclipse file data
      struct IJK
      {
        IJK() { I = -1; J = -1; K = -1; }
        IJK(int i, int j, int k) { I = i; J = j; K = k; }
        bool operator==(IJK const& other) const { return ((*this).I == other.I)&((*this).J == other.J)&((*this).K == other.K); }
        int I;
        int J;
        int K;

      };

      struct IJKHash
      {
        std::size_t operator()(const IJK& ele) const
        {
          return ele.I+ ele.J*100000+ ele.K*1000000;
        }
      };

      struct TPFA
      {
        bool operator<(TPFA const& other) const
        {
          if ((*this).downstream_index != other.downstream_index)
          {
            return (*this).downstream_index < other.downstream_index;
          }

          return (*this).upstream_index < other.upstream_index;

        }
        int downstream_index;
        int upstream_index;
        double transmissibility;

      };

      struct COMPLETION
      {
        COMPLETION(int h_index, double cf, double kh)
        {
          hosting_cell_index = h_index;
          connection_factor = cf;
          Kh = kh;
        }
        int hosting_cell_index;
        double connection_factor;
        double Kh;
      };

      struct WELL
      {
        WELL(int h_index, int nb_comp)
        {
          head_cell_index = h_index;
          nb_completions = nb_comp;
        }
        unsigned int head_cell_index;
        unsigned int nb_completions;
        std::vector<COMPLETION> completions;

      };

      bool compareTPFANNC(const TPFA& a, const TPFA& b)
      {
        return a.downstream_index > b.downstream_index;
      }

      //Grid
      std::vector<unsigned int> m_SPECGRID;
      std::vector<double>  m_COORD;
      std::vector<double>  m_ZCORN;
      std::vector<int>  m_ACTNUM;

      std::vector<TPFA>  m_NNCs;
      std::vector<TPFA>  m_EclipseGeneratedTrans;
      std::unordered_map<IJK,unsigned int, IJKHash> m_IJK2Index ;
      std::unordered_map<unsigned int,IJK> m_Index2IJK;
      std::unordered_map<unsigned int, unsigned int> m_IndexTotal2Active;

      unsigned int  m_nCOORD;
      unsigned int  m_nZCORN;
      unsigned int m_nActiveCells;
      unsigned int m_nTotalCells;
      unsigned int m_nNNCs;

      //Duplicate
      std::vector<double> m_Duplicate_Element;

      //Properties
      std::unordered_map<std::string, std::vector<double>> m_CellProperties_double;
      std::unordered_map<std::string, std::vector<int>> m_CellProperties_integer;
      std::unordered_map<std::string, std::vector<double>> m_OtherProperties_double;
      std::unordered_map<std::string, std::vector<int>> m_OtherProperties_integer;
      std::unordered_map<std::string, std::vector<char>> m_OtherProperties_char;

      //Wells
      unsigned int m_nWells;
      std::unordered_map<std::string, WELL*> m_Wells;

      std::unordered_map<int, ELEMENTS::TYPE> m_TypeMap;

      void InitElementsMapping();

      struct EGRIDDescriptor
      {
        std::string keyword;
        std::string type;
        int dim;
        int first_index;

      };

      bool m_INIT_file;
      bool m_UNRST_file;
      int m_firstSEQ;
      //Egrid

      template<class T>
        void ExtractBinaryBlock(std::string& str, int& index, int dim, int type_size, std::vector<T>& output);

      template<class T>
        void ConvertBinaryBlock(std::string keyword, std::string label_prefix, std::vector<T>& data)
        {
          utils::pamela_unused(label_prefix);
          utils::pamela_unused(data);
          LOGINFO("     o Skipping " + keyword);
        }

      void CreateAdjacencyFromTPFAdata(std::string label, std::vector<TPFA>& data, Mesh* mesh);
      void CreateEclipseGeneratedTrans();

      void CreateWellAndCompletion(Mesh* mesh);
  };

  template<>
    void Eclipse_mesh::ConvertBinaryBlock(std::string keyword, std::string label_prefix, std::vector<double>& data);
  template<>
    void Eclipse_mesh::ConvertBinaryBlock(std::string keyword, std::string label_prefix, std::vector<int>& data);
  template<>
    void Eclipse_mesh::ConvertBinaryBlock(std::string keyword, std::string label_prefix, std::vector<char>& data);


  template <class T>
    void Eclipse_mesh::ExtractBinaryBlock(std::string& str, int& index, int dim, int type_size, std::vector<T>& output)
    {
      output.reserve(dim);

      int block_size = 1000 * type_size;
      int data_size = dim * type_size;
      int nb_full_blocks = data_size / block_size;


      int n_loops;

      //Full blocks
      n_loops = block_size / type_size;
      for (auto nb = 0; nb < nb_full_blocks; ++nb)
      {
        //Skip delimiter
        index = index + 4;

        for (auto n = 0; n < n_loops; ++n)
        {

          T value = *reinterpret_cast<T *>(&str.substr(index, type_size)[0]);
          utils::bites_swap(&value);
          output.push_back(value);
          index = index + type_size;

        }

        //Skip delimiter
        index = index + 4;
      }


      //Remaining partial blocks
      block_size = dim * type_size - nb_full_blocks * block_size;

      n_loops = block_size / type_size;

      if (block_size != 0)
      {
        //Skip delimiter
        index = index + 4;
        for (auto n = 0; n < n_loops; ++n)
        {

          T value = *reinterpret_cast<T *>(&str.substr(index, type_size)[0]);
          utils::bites_swap(&value);
          output.push_back(value);
          index = index + type_size;

        }


        //Skip delimiter
        index = index + 4;
      }


    }



}
