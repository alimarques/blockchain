#include <eosiolib/eosio.hpp>
#include <eosiolib/print.hpp>

class workolab: public eosio::contract {
  public:
    workolab(account_name self)
      :eosio::contract(self),
      vototable(_self, _self)
      {}

    // @abi action
    void cadastro(account_name author, const std::string& participante) {
      vototable.emplace(author, [&](auto& new_dado) {
        new_dado.id  = vototable.available_primary_key();
        new_dado.nome = participante;
	new_dado.comp = 0.0f;
	new_dado.colaboracao = 0.0f;
	new_dado.impacto = 0.0f;
      });

      eosio::print("Participante", " cadastrado");
    }

    // @abi action
    void voto(account_name author, const uint32_t id, const uint32_t notacomp, const uint32_t notacolab, const uint32_t notaimpact) {
	auto dado_lookupvotado = vototable.find(id);
	eosio_assert(dado_lookupvotado != vototable.end(), "Este participante não existe");

	auto& nota = *dado_lookupvotado;
	float nota_anterior1 = nota.comp;
	float nota_anterior2 = nota.colaboracao;
	float nota_anterior3 = nota.impacto;
	
	vototable.modify(dado_lookupvotado, author, [&](auto& modifiable_dado) {
         modifiable_dado.comp = (nota_anterior1 + notacomp);
	 modifiable_dado.colaboracao = (nota_anterior2 + notacolab);
	 modifiable_dado.impacto = (nota_anterior3 + notaimpact);
        });

      	eosio::print("Nota", " contabilizada");
    }

    // @abi action
    void nota(account_name author, const uint32_t id) {
	auto dado_lookupvotado = vototable.find(id);
	eosio_assert(dado_lookupvotado != vototable.end(), "Este participante não existe");
	
	auto& nota = *dado_lookupvotado;
	vototable.modify(dado_lookupvotado, author, [&](auto& modifiable_dado) {
         modifiable_dado.comp = nota.comp/3;
	 modifiable_dado.colaboracao = nota.colaboracao/3;
	 modifiable_dado.impacto = nota.impacto/3;
        });

      	eosio::print("Nota", " calculada");
    }


  private:
    // @abi table vototable i64
    struct vote1 {
      uint64_t id;
      std::string nome;
      uint64_t comp;
      uint64_t colaboracao;
      uint64_t impacto;


      uint64_t primary_key() const { return id; }
      EOSLIB_SERIALIZE(vote1, (id)(nome)(comp)(colaboracao)(impacto))
    };

    typedef eosio::multi_index<N(vototable), vote1> votacao_table;
    votacao_table vototable;

};

EOSIO_ABI(workolab, (cadastro)(voto)(nota))

