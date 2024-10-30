from substrateinterface import SubstrateInterface
from scalecodec.base import ScaleBytes

substrate = SubstrateInterface(url="wss://kusama.rpc.robonomics.network")
res_block = substrate.rpc_request("chain_getBlock", ["0x97ff645b2035a0ad62ed5f438ebd5ee91cbfe3d197ba221c6c03c614c6dc1dfe"])
extrinsic = res_block["result"]["block"]["extrinsics"][3] # Тут будет записано то, что в файле encoded_extrinsic
print(f"Encoded extrinsic: \n{extrinsic}")

substrate.init_runtime()
extrinsic_cls = substrate.runtime_config.get_decoder_class('Extrinsic')
extrinsic_decoder = extrinsic_cls(data=ScaleBytes(extrinsic), metadata = substrate.metadata, runtime_config = substrate.runtime_config)
decoced = extrinsic_decoder.decode(check_remaining=substrate.config.get('strict_scale_decode'))

print("Type is: ", extrinsic_decoder.sub_type)

print(f"Decoded extrinsic: \n{decoced}")